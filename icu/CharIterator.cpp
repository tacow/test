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
    if (!str_ || pos_ == len_)
        return NULL;

    bool valid = true;
    const UChar* res = NULL;
    do {
        UChar32 u32 = 0;
        *charLen = 1;
        if (IsLeadSurrogate(str_[pos_])) {
            if (pos_ + 1 < len_ && IsTrailSurrogate(str_[pos_ + 1])) {
                u32 = AssembleSurrogatePair(str_[pos_], str_[pos_ + 1]);
                *charLen = 2;
            } else {
                valid = false;
            }
        } else if (IsTrailSurrogate(str_[pos_])) {
            valid = false;
        } else {
            u32 = str_[pos_];
        }
        if (valid) {
            *charWidth = wcwidth(u32);
            res = &str_[pos_];
        }
        pos_ += *charLen;
    } while (!valid && pos_ < len_);
    return res;
}

