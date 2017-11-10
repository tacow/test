#ifndef CHARSET_DETECTOR_H
#define CHARSET_DETECTOR_H

#include <string>

class CharsetDetector {
public:
    CharsetDetector();
    ~CharsetDetector();

    bool CodePageInput(const char* data, int len, std::string& charset);

private:
    void DetectCharset();

    bool detected_;
    std::string charset_;
    char* detectBuf_;
    int detectLen_;
    int nonASCIIByteCount_;
};

#endif

