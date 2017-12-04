#include "CharIterator.h"
#include <wchar.h>

#define IsLeadSurrogate(u)  ((u) >= 0xD800 && (u) <= 0xDBFF)
#define IsTrailSurrogate(u) ((u) >= 0xDC00 && (u) <= 0xDFFF)
#define AssembleSurrogatePair(u1,u2) (((((UChar32)(u1) & 0x03FF) << 10) | ((UChar32)(u2) & 0x03FF)) + (UChar32)0x10000)

CharIterator::CharIterator() {
    str_ = NULL;
    len_ = 0;
    pos_ = 0;
}

CharIterator::~CharIterator() {
}

void CharIterator::SetString(const UChar* str, int len) {
    str_ = str;
    len_ = len;
    pos_ = 0;
}

const UChar* CharIterator::GetNextChar(int* charLen, int* charWidth) {
    *charLen = 0;
    *charWidth = 0;
    if (!str_ || pos_ == len_)
        return NULL;

    const UChar* res = &str_[pos_];
    do {
        bool valid = true;
        UChar32 code = 0;
        int codeLen = 0;
        if (IsLeadSurrogate(str_[pos_])) {
            if (pos_ + 1 < len_ && IsTrailSurrogate(str_[pos_ + 1])) {
                code = AssembleSurrogatePair(str_[pos_], str_[pos_ + 1]);
                codeLen = 2;
            } else {
                valid = false;
            }
        } else if (IsTrailSurrogate(str_[pos_])) {
            valid = false;
        } else {
            code = str_[pos_];
            codeLen = 1;
        }
        if (valid) {
            int codeWidth = wcwidth(code);
            if (codeWidth < 0) {
                if (*charLen == 0) {
                    *charLen = codeLen;
                    *charWidth = codeWidth;
                    pos_ += codeLen;
                }
                break;
            } else if (codeWidth == 0) {
                *charLen += codeLen;
                pos_ += codeLen;
            } else {
                if (*charLen == 0) {
                    *charLen = codeLen;
                    *charWidth = codeWidth;
                    pos_ += codeLen;
                } else {
                    break;
                }
            }
        } else {
            if (*charLen == 0) {
                pos_ += 1;
                return NULL;
            } else {
                break;
            }
        }
    } while (pos_ < len_);
    return res;
}

