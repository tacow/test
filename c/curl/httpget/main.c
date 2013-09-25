#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

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

int HttpGet(const char* url, char* contentBuf, size_t* bufLen) {
    CURL* curl;
    CURLcode res;
    long resCode;
    struct Buffer buffer;
    int ret = -1;

    buffer.buf = contentBuf;
    buffer.size = 0;
    buffer.maxSize = *bufLen;

    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url);
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
    return ret;
}

int main(int argc, char* argv[]) {
    char* url;
    char* contentBuf;
    size_t bufLen;
    int res;

    if (argc < 2) {
        printf("%s [url]\n", argv[0]);
        return 0;
    }
    url = argv[1];

    curl_global_init(CURL_GLOBAL_ALL);

    contentBuf = malloc(CONTENT_LEN);
    bufLen = CONTENT_LEN - 1;
    res = HttpGet(url, contentBuf, &bufLen);
    if (res == 0) {
        contentBuf[bufLen] = '\0';
        puts(contentBuf);
    }
    free(contentBuf);

    curl_global_cleanup();
    return 0;
}

