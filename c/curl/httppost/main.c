#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#define FILE_LEN (10 * 1024 * 1024)
#define CONTENT_LEN (1024 * 1024)

struct Buffer {
    char* buf;
    size_t size;
    size_t maxSize;
};

size_t WriteFunc(char* ptr, size_t size, size_t nmemb, void* userdata) {
    struct Buffer* buffer = userdata;
    size_t writeLen = size * nmemb;

    if (buffer->size + writeLen > buffer->maxSize)
        writeLen = buffer->maxSize - buffer->size;
    memcpy(buffer->buf + buffer->size, ptr, writeLen);
    buffer->size += writeLen;
    return size * nmemb;
}

int HttpPost(const char* url, const void* data, size_t dataLen, char* contentBuf, size_t* bufLen) {
    CURL* curl;
    struct curl_slist* headerlist = NULL;
    CURLcode res;
    long resCode;
    struct Buffer buffer;
    int ret = -1;

    headerlist = curl_slist_append(headerlist, "Content-Type: application/octet-stream");
    headerlist = curl_slist_append(headerlist, "Expect:");

    buffer.buf = contentBuf;
    buffer.size = 0;
    buffer.maxSize = *bufLen;

    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)dataLen);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteFunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    res = curl_easy_perform(curl);
    if (res == CURLE_OK) {
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &resCode);
        if (200 == resCode) {
            *bufLen = buffer.size;
            ret = 0;
        }
    }
    curl_easy_cleanup(curl);
    curl_slist_free_all(headerlist);
    return ret;
}

int main(int argc, char* argv[]) {
    char* url;
    FILE* f;
    char* fileBuf;
    size_t fileLen;
    char* contentBuf;
    size_t bufLen;
    int res;

    if (argc < 3) {
        printf("%s [url] [data file]\n", argv[0]);
        return 0;
    }
    url = argv[1];

    f = fopen(argv[2], "r");
    if (!f) {
        printf("Failed to open file %s\n", argv[2]);
        return 0;
    }
    fileBuf = malloc(FILE_LEN);
    fileLen = fread(fileBuf, 1, FILE_LEN, f);
    fclose(f);

    curl_global_init(CURL_GLOBAL_ALL);

    contentBuf = malloc(CONTENT_LEN);
    bufLen = CONTENT_LEN - 1;
    res = HttpPost(url, fileBuf, fileLen, contentBuf, &bufLen);
    if (res == 0) {
        contentBuf[bufLen] = '\0';
        puts(contentBuf);
    }
    free(contentBuf);
    free(fileBuf);

    curl_global_cleanup();
    return 0;
}

