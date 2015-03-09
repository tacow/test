#include "CharsetConvert.h"
#include <errno.h>

CharsetConvert::CharsetConvert() {
    m_cd = (iconv_t)-1;

    m_buf = NULL;
    m_bufSize = 0;
}

CharsetConvert::~CharsetConvert() {
    if (m_cd != (iconv_t)-1)
        iconv_close(m_cd);

    if (m_buf)
        delete[] m_buf;
}

bool CharsetConvert::Init(const char* fromCharset, const char* toCharset, size_t bufSize) {
    m_buf = new char[bufSize];
    m_bufSize = bufSize;

    m_cd = iconv_open(toCharset, fromCharset);
    return m_cd != (iconv_t)-1;
}

bool CharsetConvert::Convert(const string& fromStr, string& toStr) {
    char*  inBuf = (char*)fromStr.c_str();
    size_t inBufLen = fromStr.size();
    do {
        char*  outBuf = m_buf;
        size_t outBufLen = m_bufSize;
        size_t res = iconv(m_cd, &inBuf, &inBufLen, &outBuf, &outBufLen);
        if (res == (size_t)-1 && errno != E2BIG) {
            iconv(m_cd, NULL, NULL, NULL, NULL);
            return false;
        }
        toStr.append(m_buf, m_bufSize - outBufLen);
    } while(inBufLen > 0);
    return true;
}

