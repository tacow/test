#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

int main(int argc, char* argv[]) {
    CURL* curl;
    char line[4096];
    char* encoded = NULL;

    fgets(line, 4096, stdin);
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    encoded = curl_easy_escape(curl, line, strlen(line));
    printf("%s\n", encoded);
    curl_free(encoded);
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return 0;
}

