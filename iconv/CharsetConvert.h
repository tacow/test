#ifndef CHARSET_CONVERT_H
#define CHARSET_CONVERT_H

#include <iconv.h>

enum ConvertResult {
    kConvertOK = 0,     // 转换成功
    kCantContinue = 1,  // 无法继续转换（输入数据不足一个字符或输出空间不足）
    kInvalidInput = 2   // 输入数据格式错误
};

class CharsetConvert {
public:
    CharsetConvert();
    ~CharsetConvert();

    bool Init(const char* toCharset, const char* fromCharset);
    void Reset();

    ConvertResult Convert(const char** fromStr, size_t* fromStrLen, char** toStr, size_t* toStrLen);

private:
    iconv_t m_cd;
};

#endif

