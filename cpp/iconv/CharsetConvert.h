#ifndef CHARSET_CONVERT_H
#define CHARSET_CONVERT_H

#include <iconv.h>
#include <string>

using namespace std;

class CharsetConvert {
public:
    CharsetConvert();
    ~CharsetConvert();

    bool Init(const char* toCharset, const char* fromCharset, size_t bufSize = 1024 * 1024);
    bool Convert(const string& fromStr, string& toStr);
private:
    iconv_t m_cd;

    char*  m_fromBuf;
    char*  m_toBuf;
    size_t m_bufSize;
};

#endif

