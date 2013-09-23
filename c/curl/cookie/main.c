#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

size_t WriteFunc(char* ptr, size_t size, size_t nmemb, void* buf) {
    return size * nmemb;
}

void HttpGet(const char* url) {
    CURL* curl;
    CURLcode res;
    struct curl_slist* cookies = NULL;
    char buf[4096];
    char* ptr = NULL;
    size_t i;
    char* token;
    char* name;
    char* value;

    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteFunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, NULL);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");
    res = curl_easy_perform(curl);
    if (res == CURLE_OK) {
        curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &cookies);
        while(cookies) {
            name = NULL;
            value = NULL;
            snprintf(buf, 4096, "%s", cookies->data);
            strtok_r(buf, "\t", &ptr);
            for(i = 0; i < 6; ++i) {
                token = strtok_r(NULL, "\t", &ptr);
                if (token == NULL)
                    break;
                if (i == 4)
                    name = token;
                if (i == 5)
                    value = token;
            }
            if (name && value)
                printf("%s=%s\n", name, value);
            cookies = cookies->next;
        }
        curl_slist_free_all(cookies);
    }
    curl_easy_cleanup(curl);
}

int main(int argc, char* argv[]) {
    char* url = NULL;

    if (argc < 2) {
        printf("%s [url]\n", argv[0]);
        return 0;
    }

    url = argv[1];
    HttpGet(url);
    return 0;
}

