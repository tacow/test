#include "CharsetConvertStream.h"
#include <string.h>
#include <algorithm>

using namespace std;

CharsetConvertStream::CharsetConvertStream() {
    readBufLen_ = 0;
}

CharsetConvertStream::~CharsetConvertStream() {
}

bool CharsetConvertStream::Init(const char* toCharset, const char* fromCharset) {
    return converter_.Init(toCharset, fromCharset);
}

bool CharsetConvertStream::Process(const char* str, size_t len) {
    if (readBufLen_ > 0) {  // 有字符未被转换
        size_t copyLen = min(READ_BUF_SIZE - readBufLen_, len);
        memcpy(readBuf_ + readBufLen_, str, copyLen);
        readBufLen_ += copyLen;

        const char* readPtr = readBuf_;
        char* convertPtr = convertBuf_;
        size_t convertLen = CONVERT_BUF_SIZE;
        ConvertResult result = converter_.Convert(&readPtr, &readBufLen_, &convertPtr, &convertLen);
        if (result == kInvalidInput) {
            readBufLen_ = 0;
            return false;
        } else if (result == kCantContinue) {
            if (readBufLen_ == READ_BUF_SIZE) {
                readBufLen_ = 0;
                return false;
            }
            return true;
        }
        OnConvertedChars(convertBuf_, CONVERT_BUF_SIZE - convertLen);

        size_t advanceLen = copyLen > readBufLen_ ? copyLen - readBufLen_ : 0;
        str += advanceLen;
        len -= advanceLen;
        readBufLen_ = 0;
    }

    while (len > 0) {
        char* convertPtr = convertBuf_;
        size_t convertLen = CONVERT_BUF_SIZE;
        ConvertResult result = converter_.Convert(&str, &len, &convertPtr, &convertLen);
        if (result == kInvalidInput) {
            readBufLen_ = 0;
            return false;
        } else if (result == kCantContinue) {
            if (len > 0) {
                if (len >= READ_BUF_SIZE) {
                    readBufLen_ = 0;
                    return false;
                } else {
                    memcpy(readBuf_, str, len);
                    readBufLen_ = len;
                    return true;
                }
            }
        }
        OnConvertedChars(convertBuf_, CONVERT_BUF_SIZE - convertLen);
    }
    return true;
}

