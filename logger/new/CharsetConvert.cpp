#include "CharsetConvert.h"
#include <string.h>

CharsetConvert::CharsetConvert() {
    m_cd = (iconv_t)-1;

    m_fromBuf = NULL;
    m_toBuf = NULL;
    m_bufSize = 0;
}

CharsetConvert::~CharsetConvert() {
    if (m_cd != (iconv_t)-1)
        iconv_close(m_cd);

    if (m_fromBuf)
        delete[] m_fromBuf;
    if (m_toBuf)
        delete[] m_toBuf;
}

bool CharsetConvert::Init(const char* toCharset, const char* fromCharset, size_t bufSize) {
    m_fromBuf = new char[bufSize];
    m_toBuf = new char[bufSize];
    m_bufSize = bufSize;

    m_cd = iconv_open(toCharset, fromCharset);
    return m_cd != (iconv_t)-1;
}

char* CharsetConvert::Convert(char* fromStr, size_t fromStrLen, size_t* toStrLen) {
    if (fromStrLen == size_t(-1))
        fromStrLen = strlen(fromStr);

    char*  toStrPtr = m_toBuf;
    size_t bytesLeft = m_bufSize - 1;
    size_t res = iconv(m_cd, &fromStr, &fromStrLen, &toStrPtr, &bytesLeft);
    if (res == (size_t)-1) {
        iconv(m_cd, NULL, NULL, NULL, NULL);
        return NULL;
    }
    if (fromStrLen != 0)
        return NULL;

    *toStrPtr = '\0';
    if (toStrLen)
        *toStrLen = toStrPtr - m_toBuf;
    return m_toBuf;
}

bool CharsetConvert::Convert(const string& fromStr, string& toStr) {
    size_t fromStrLen = fromStr.size();
    if (fromStrLen > m_bufSize - 1)
        return false;
    memcpy(m_fromBuf, fromStr.c_str(), fromStrLen);
    m_fromBuf[fromStrLen] = '\0';

    size_t toStrLen;
    char* toStrPtr = Convert(m_fromBuf, fromStrLen, &toStrLen);
    if (!toStrPtr)
        return false;
    toStr.assign(toStrPtr, toStrLen);
    return true;
}

