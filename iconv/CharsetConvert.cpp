#include "CharsetConvert.h"
#include <string.h>
#include <errno.h>

CharsetConvert::CharsetConvert() {
    m_cd = (iconv_t)-1;
}

CharsetConvert::~CharsetConvert() {
    if (m_cd != (iconv_t)-1)
        iconv_close(m_cd);
}

bool CharsetConvert::Init(const char* toCharset, const char* fromCharset) {
    m_cd = iconv_open(toCharset, fromCharset);
    return m_cd != (iconv_t)-1;
}

void CharsetConvert::Reset() {
    iconv(m_cd, NULL, NULL, NULL, NULL);
}

ConvertResult CharsetConvert::Convert(const char** fromStr, size_t* fromStrLen, char** toStr, size_t* toStrLen) {
    size_t oriLen = *fromStrLen;
    size_t res = iconv(m_cd, (char**)fromStr, fromStrLen, toStr, toStrLen);
    if (res == (size_t)-1 && errno == EILSEQ) {
        Reset();
        return kInvalidInput;
    }

    if (*fromStrLen == oriLen)
        return kCantContinue;

    return kConvertOK;
}

