#include "CharsetDetector.h"
#include <string.h>
#include <algorithm>
#include <unicode/ucsdet.h>

#define DEFAULT_CHARSET "UTF-8"
#define DETECT_MAX_LEN 131072
#define DETECT_MIN_NON_ASCII_LEN 6
#define DETECT_MIN_SCORE 10

CharsetDetector::CharsetDetector() {
    detected_ = false;
    detectBuf_ = new char[DETECT_MAX_LEN];
    detectLen_ = 0;
    nonASCIIByteCount_ = 0;
}

CharsetDetector::~CharsetDetector() {
    if (detectBuf_)
        delete[] detectBuf_;
}

void CharsetDetector::CodePageInput(const char* data, int len) {
    if (detected_)
        return;

    bool needDetect = false;
    int copyLen = std::min(len, DETECT_MAX_LEN - detectLen_);
    memcpy(detectBuf_ + detectLen_, data, copyLen);
    detectLen_ += copyLen;
    if (detectLen_ == DETECT_MAX_LEN)
        needDetect = true;
    if (!needDetect) {
        for (int i = 0; i < copyLen; ++i) {
            if ((data[i] & 0x80)) {
                ++nonASCIIByteCount_;
            }
            if (nonASCIIByteCount_ >= DETECT_MIN_NON_ASCII_LEN) {
                needDetect = true;
                break;
            }
        }
    }
    if (needDetect) {
        DetectCharset();
        detected_ = true;
        delete[] detectBuf_;
        detectBuf_ = NULL;
        detectLen_ = 0;
        nonASCIIByteCount_ = 0;
    }
}

void CharsetDetector::DetectCharset() {
    bool succ = false;
    UErrorCode err = U_ZERO_ERROR;
    UCharsetDetector* detector = ucsdet_open(&err);
    ucsdet_setText(detector, detectBuf_, detectLen_, &err);
    const UCharsetMatch* match = ucsdet_detect(detector, &err);
    if (match) {
        int score = ucsdet_getConfidence(match, &err);
        if (score >= DETECT_MIN_SCORE) {
            const char* charset = ucsdet_getName(match, &err);
            if (charset && *charset) {
                succ = true;
                OnDetecedCharset(charset);
            }
        }
    }
    if (!succ)
        OnDetecedCharset(DEFAULT_CHARSET);
    ucsdet_close(detector);
}

