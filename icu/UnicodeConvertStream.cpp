#include "UnicodeConvertStream.h"

#define TO_UNICODE_BUF_LEN 1024
#define FROM_UNICODE_BUF_LEN 4096

UnicodeConvertStream::UnicodeConvertStream() {
    converter_ = NULL;
    toUnicodeBuf_ = new UChar[TO_UNICODE_BUF_LEN];
    fromUnicodeBuf_ = new char[FROM_UNICODE_BUF_LEN];
}

UnicodeConvertStream::~UnicodeConvertStream() {
    if (converter_)
        ucnv_close(converter_);
    delete[] toUnicodeBuf_;
    delete[] fromUnicodeBuf_;
}

bool UnicodeConvertStream::SetCharset(const char* charset) {
    if (converter_) {
        ucnv_close(converter_);
        converter_ = NULL;
    }
    UErrorCode err = U_ZERO_ERROR;
    converter_ = ucnv_open(charset, &err);
    return (converter_ != NULL);
}

void UnicodeConvertStream::CodePageInput(const char* data, int len, bool flush) {
    UErrorCode err = U_ZERO_ERROR;
    const char* source = data;
    do {
        UChar* target = toUnicodeBuf_;

        err = U_ZERO_ERROR;
        ucnv_toUnicode(converter_,
                       &target, target + TO_UNICODE_BUF_LEN,
                       &source, data + len,
                       NULL, (UBool)flush, &err);
        if (target > toUnicodeBuf_)
            OnUnicodeOutput(toUnicodeBuf_, target - toUnicodeBuf_);
    } while (err == U_BUFFER_OVERFLOW_ERROR);
}

void UnicodeConvertStream::UnicodeInput(const UChar* data, int len, bool flush) {
    UErrorCode err = U_ZERO_ERROR;
    const UChar* source = data;
    do {
        char* target = fromUnicodeBuf_;

        err = U_ZERO_ERROR;
        ucnv_fromUnicode(converter_,
                &target, target + FROM_UNICODE_BUF_LEN,
                &source, data + len,
                NULL, (UBool)flush, &err);
        if (target > fromUnicodeBuf_)
            OnCodePageOutput(fromUnicodeBuf_, target - fromUnicodeBuf_);
    } while (err == U_BUFFER_OVERFLOW_ERROR);
}

void UnicodeConvertStream::OnUnicodeOutput(UChar* str, int len) {
}

void UnicodeConvertStream::OnCodePageOutput(char* str, int len) {
}

