#ifndef CHARSET_DETECTOR_H
#define CHARSET_DETECTOR_H

class CharsetDetector {
public:
    CharsetDetector();
    ~CharsetDetector();

    void CodePageInput(const char* data, int len);

    virtual void OnDetecedCharset(const char* charset) = 0;

private:
    void DetectCharset();

    bool detected_;
    char* detectBuf_;
    int detectLen_;
    int nonASCIIByteCount_;
};

#endif

