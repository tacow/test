#include "CharsetConvert.h"
#include <string.h>

CharsetConvert::CharsetConvert() {
    m_cd = (iconv_t)-1;

    m_fromBuf = NULL;
    m_toBuf = NULL;
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

bool CharsetConvert::Convert(const string& fromStr, string& toStr) {
    char*  fromStrPtr = m_fromBuf;
    size_t fromStrLen = fromStr.size();
    char*  toStrPtr = m_toBuf;
    size_t toStrLen = m_bufSize;
    if (fromStrLen > m_bufSize)
        return false;
    memcpy(m_fromBuf, fromStr.c_str(), fromStrLen);
    size_t res = iconv(m_cd, &fromStrPtr, &fromStrLen, &toStrPtr, &toStrLen);
    if (res == (size_t)-1 || fromStrLen != 0)
        return false;
    toStrLen = m_bufSize - toStrLen;
    toStr.assign(m_toBuf, toStrLen);
    return true;
}

