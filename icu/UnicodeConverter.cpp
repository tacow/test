#include "UnicodeConverter.h"

UnicodeConverter::UnicodeConverter(int toUnicodeBufLen, int fromUnicodeBufLen) {
    converter_ = NULL;
    toUnicodeBufLen_ = toUnicodeBufLen;
    toUnicodeBuf_ = new UChar[toUnicodeBufLen];
    fromUnicodeBufLen_ = fromUnicodeBufLen;
    fromUnicodeBuf_ = new char[fromUnicodeBufLen];
}

UnicodeConverter::~UnicodeConverter() {
    if (converter_)
        ucnv_close(converter_);
    delete[] toUnicodeBuf_;
    delete[] fromUnicodeBuf_;
}

bool UnicodeConverter::SetCharset(const char* charset) {
    if (converter_) {
        ucnv_close(converter_);
        converter_ = NULL;
    }
    UErrorCode err = U_ZERO_ERROR;
    converter_ = ucnv_open(charset, &err);
    return (converter_ != NULL);
}

UChar* UnicodeConverter::ConvertToUnicode(const char* data, int len, int* destLen) {
    UErrorCode err = U_ZERO_ERROR;
    *destLen = ucnv_toUChars(converter_, toUnicodeBuf_, toUnicodeBufLen_, data, len, &err);
    if (*destLen > toUnicodeBufLen_)
        *destLen = toUnicodeBufLen_;
    return toUnicodeBuf_;
}

char* UnicodeConverter::ConvertFromUnicode(const UChar* data, int len, int* destLen) {
    UErrorCode err = U_ZERO_ERROR;
    *destLen = ucnv_fromUChars(converter_, fromUnicodeBuf_, fromUnicodeBufLen_, data, len, &err);
    if (*destLen > fromUnicodeBufLen_)
        *destLen = fromUnicodeBufLen_;
    return fromUnicodeBuf_;
}

