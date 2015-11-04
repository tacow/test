#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <string>
#include <deque>
#include <curl/curl.h>
#include "StreamWriter.h"

using namespace std;

struct HttpHeader {
    string name;
    string value;
};

class HttpClient {
public:
    static bool GlobalInit();
    static void GlobalCleanup();

    bool HttpGet(const char* url,
                 deque<HttpHeader>* reqHeaders, deque<HttpHeader>* resHeaders,
                 void* contentBuf, size_t bufSize, long timeout);

    bool HttpPost(const char* url,
                  deque<HttpHeader>* reqHeaders, deque<HttpHeader>* resHeaders,
                  const void* postData, size_t postDataLength,
                  void* contentBuf, size_t bufSize, long timeout);

    long GetStatusCode();
    size_t GetContentLength();
    const char* GetErrMsg();

private:
    StreamWriter m_streamWriter;
    size_t m_contentLength;
    long m_statusCode;
    char m_errMsg[256];

    static size_t HeaderFuncS(char* ptr, size_t size, size_t nmemb, deque<HttpHeader>* resHeaders);

    static size_t WriteFuncS(char* ptr, size_t size, size_t nmemb, HttpClient* httpClient);
    size_t WriteFunc(char* ptr, size_t size, size_t nmemb);
};

void UrlEncode(const string& url, string& encoded);

#endif

