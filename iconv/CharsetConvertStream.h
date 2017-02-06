#ifndef CHARSET_CONVERT_STREAM_H
#define CHARSET_CONVERT_STREAM_H

#include "CharsetConvert.h"

#define CONVERT_BUF_SIZE 4096
#define READ_BUF_SIZE    16

class CharsetConvertStream {
public:
    CharsetConvertStream();
    ~CharsetConvertStream();

    bool Init(const char* toCharset, const char* fromCharset);

    // 返回值代表是否成功（若出现异常字节序列则返回false）
    bool Process(const char* str, size_t len);

protected:
    virtual void OnConvertedChars(char* str, size_t len) = 0;

private:
    CharsetConvert converter_;
    char convertBuf_[CONVERT_BUF_SIZE];
    char readBuf_[READ_BUF_SIZE];
    size_t readBufLen_;
};

#endif

