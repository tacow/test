#ifndef UNICODE_CONVERT_STREAM_H
#define UNICODE_CONVERT_STREAM_H

#include <unicode/ucnv.h>

class UnicodeConvertStream {
public:
    UnicodeConvertStream();
    ~UnicodeConvertStream();

    bool SetCharset(const char* charset);

    void CodePageInput(const char* data, int len, bool flush);
    void UnicodeInput(const UChar* data, int len, bool flush);

    virtual void OnUnicodeOutput(UChar* str, int len);
    virtual void OnCodePageOutput(char* str, int len);

private:
    UConverter* converter_;
    UChar* toUnicodeBuf_;
    char* fromUnicodeBuf_;
};

#endif

