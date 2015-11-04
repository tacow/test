#include "HttpClient.h"
#include "stl_util.h"

bool HttpClient::GlobalInit() {
    return CURLE_OK == curl_global_init(CURL_GLOBAL_ALL);
}

void HttpClient::GlobalCleanup() {
    curl_global_cleanup();
}

bool HttpClient::HttpGet(const char* url,
                         deque<HttpHeader>* reqHeaders, deque<HttpHeader>* resHeaders,
                         void* contentBuf, size_t bufSize, long timeout) {
    m_streamWriter.Init(contentBuf, bufSize);
    m_contentLength = 0;
    m_statusCode = 0;
    m_errMsg[0] = '\0';

    CURL* curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteFuncS);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

    curl_slist* slist = NULL;
    if (reqHeaders) {
        for(deque<HttpHeader>::iterator it = reqHeaders->begin(); it != reqHeaders->end(); ++it) {
            HttpHeader& header = *it;
            string headerStr = header.name + ": " + header.value;
            slist = curl_slist_append(slist, headerStr.c_str());
        }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
    }

    if (resHeaders) {
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, HeaderFuncS);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, resHeaders);
    }

    CURLcode res = curl_easy_perform(curl);
    bool succ = (res == CURLE_OK);
    if (succ)
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &m_statusCode);
    else
        snprintf(m_errMsg, 256, "%s", curl_easy_strerror(res));

    if (slist)
        curl_slist_free_all(slist);
    curl_easy_cleanup(curl);
    return succ;
}

bool HttpClient::HttpPost(const char* url,
                          deque<HttpHeader>* reqHeaders, deque<HttpHeader>* resHeaders,
                          const void* postData, size_t postDataLength,
                          void* contentBuf, size_t bufSize, long timeout) {
    m_streamWriter.Init(contentBuf, bufSize);
    m_contentLength = 0;
    m_statusCode = 0;
    m_errMsg[0] = '\0';

    CURL* curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteFuncS);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)postDataLength);

    curl_slist* slist = NULL;
    if (reqHeaders) {
        for(deque<HttpHeader>::iterator it = reqHeaders->begin(); it != reqHeaders->end(); ++it) {
            HttpHeader& header = *it;
            string headerStr = header.name + ": " + header.value;
            slist = curl_slist_append(slist, headerStr.c_str());
        }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
    }

    if (resHeaders) {
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, HeaderFuncS);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, resHeaders);
    }

    CURLcode res = curl_easy_perform(curl);
    bool succ = (res == CURLE_OK);
    if (succ)
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &m_statusCode);
    else
        snprintf(m_errMsg, 256, "%s", curl_easy_strerror(res));

    if (slist)
        curl_slist_free_all(slist);
    curl_easy_cleanup(curl);
    return succ;
}

long HttpClient::GetStatusCode() {
    return m_statusCode;
}

size_t HttpClient::GetContentLength() {
    return m_contentLength;
}

const char* HttpClient::GetErrMsg() {
    return m_errMsg;
}

size_t HttpClient::HeaderFuncS(char* ptr, size_t size, size_t nmemb, deque<HttpHeader>* resHeaders) {
    int len = size * nmemb;
    string headerStr(ptr, len);
    size_t pos = headerStr.find(":");
    if (pos != string::npos) {
        HttpHeader header;
        header.name = Trim(headerStr.substr(0, pos));
        header.value = Trim(headerStr.substr(pos + 1, len - pos - 1));
        resHeaders->push_back(header);
    }
    return len;
}

size_t HttpClient::WriteFuncS(char* ptr, size_t size, size_t nmemb, HttpClient* httpClient) {
    return httpClient->WriteFunc(ptr, size, nmemb);
}

size_t HttpClient::WriteFunc(char* ptr, size_t size, size_t nmemb) {
    size_t bytesToWrite = size * nmemb;
    m_streamWriter.Write(ptr, bytesToWrite);
    m_contentLength += bytesToWrite;
    return bytesToWrite;
}

void UrlEncode(const string& url, string& encoded) {
    CURL* curl = curl_easy_init();
    char* encodedStr = curl_easy_escape(curl, url.c_str(), url.size());
    encoded = encodedStr;
    curl_free(encodedStr);
    curl_easy_cleanup(curl);
}

