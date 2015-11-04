#include "HttpClient.h"
#include <stdio.h>

#define CONTENT_BUF_SIZE (1024 * 1024)

void HttpGetTest(const char* url) {
    deque<HttpHeader> reqHeaders;
    deque<HttpHeader> resHeaders;
    char* contentBuf = new char[CONTENT_BUF_SIZE + 1];

    HttpHeader header;
    header.name = "User-Agent";
    header.value = "Curl";
    reqHeaders.push_back(header);

    header.name = "Accept";
    header.value = "text/html,*/*;q=0.8";
    reqHeaders.push_back(header);

    header.name = "Accept-Language";
    header.value = "zh-CN,zh;q=0.8";
    reqHeaders.push_back(header);

    header.name = "Accept-Charset";
    header.value = "gb18030,gb2312,gbk";
    reqHeaders.push_back(header);

    string errMsg;
    HttpClient httpClient;
    if (httpClient.HttpGet(url, &reqHeaders, &resHeaders, contentBuf, CONTENT_BUF_SIZE, 3)) {
        long statusCode = httpClient.GetStatusCode();
        size_t contentLength = httpClient.GetContentLength();
        if (statusCode != 200) {
            char errBuf[256];
            snprintf(errBuf, 256, "HTTP status code: %ld", statusCode);
            errMsg = errBuf;
        } else if (contentLength > CONTENT_BUF_SIZE) {
            char errBuf[256];
            snprintf(errBuf, 256, "Content length: %d", (int)contentLength);
            errMsg = errBuf;
        }
        contentBuf[contentLength] = '\0';
    } else {
        errMsg = httpClient.GetErrMsg();
    }

    if (!errMsg.empty()) {
        printf("Request URL %s failed: %s\n", url, errMsg.c_str());
    } else {
        printf("[Headers]\n");
        for(deque<HttpHeader>::iterator it = resHeaders.begin(); it != resHeaders.end(); ++it) {
            HttpHeader& header = *it;
            printf("\"%s\": \"%s\"\n", header.name.c_str(), header.value.c_str());
        }
        printf("\n[Content]\n%s", contentBuf);
    }

    delete[] contentBuf;
}

int main() {
    HttpClient::GlobalInit();

    HttpGetTest("http://nginx.org/");
    HttpGetTest("http://www.baidu.com/");

    HttpClient::GlobalCleanup();
    return 0;
}

