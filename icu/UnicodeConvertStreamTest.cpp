// g++ -g -o UnicodeConvertStreamTest UnicodeConvertStreamTest.cpp CharsetDetector.cpp UnicodeConvertStream.cpp CharIterator.cpp -licui18n

#include <stdio.h>
#include <string.h>
#include <string>
#include "CharsetDetector.h"
#include "UnicodeConvertStream.h"
#include "CharIterator.h"

#define DEFAULT_CHARSET "UTF-8"

FILE* inF = NULL;
FILE* outF = NULL;

void WriteUChar(const UChar* str, int len) {
    fwrite(str, sizeof(UChar), len, outF);
}

void WriteData(const char* str, int len) {
    fwrite(str, sizeof(char), len, outF);
}

class UnicodeConvertStreamTest : public UnicodeConvertStream {
public:
    void OnUnicodeOutput(UChar* str, int len) {
        ci_.SetString(str, len);
        const UChar* ch;
        int chLen, chWidth;
        while ((ch = ci_.GetNextChar(&chLen, &chWidth))) {
            if (chWidth > 0) {
                UChar sp = ' ';
                WriteUChar(ch, len);
                WriteUChar(&sp, 1);
            }
        }
    }

    void OnCodePageOutput(char* str, int len) {
        WriteData(str, len);
    }

private:
    CharIterator ci_;
};

int main(int argc, char** argv) {
    if (argc < 3) {
        printf("Usage:\n"
                "  %s <charset> [infile] [outfile]       -- From codepage to UNICODE\n"
                "  %s -r <charset> [infile] [outfile]    -- From UNICODE to codepage\n", argv[0], argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "-r") != 0) {
        const char* charset = NULL;
        const char* infile = NULL;
        const char* outfile = NULL;
        if (argc > 3) {
            charset = argv[1];
            infile = argv[2];
            outfile = argv[3];
        } else {
            infile = argv[1];
            outfile = argv[2];
        }

        bool charsetDetected = false;
        bool charsetSet = false;
        CharsetDetector detector;
        UnicodeConvertStreamTest stream;
        if (charset) {
            if (stream.SetCharset(charset)) {
                charsetDetected = true;
                charsetSet = true;
            } else {
                printf("Unsupported charset: %s, will detect\n", charset);
            }
        }

        inF = fopen(infile, "rb");
        outF = fopen(outfile, "wb");
        if (!inF || !outF) {
            printf("Can't open file\n");
            return 1;
        }
        UChar bom = 0xfeff;
        WriteUChar(&bom, 1);

        char inBuf[4096];
        int n;
        while ((n = fread(inBuf, 1, 4096, inF)) > 0) {
            if (!charsetDetected) {
                std::string detectedCharset;
                charsetDetected = detector.CodePageInput(inBuf, n, detectedCharset);
                if (charsetDetected) {
                    printf("Detected charset: %s\n", detectedCharset.c_str());
                    stream.SetCharset(detectedCharset.c_str());
                    charsetSet = true;
                }
            }
            if (!charsetSet) {
                stream.SetCharset(DEFAULT_CHARSET);
                charsetSet = true;
            }
            stream.CodePageInput(inBuf, n, false);
        }

        fclose(inF);
        fclose(outF);
    } else {
        const char* charset = NULL;
        const char* infile = NULL;
        const char* outfile = NULL;
        if (argc > 4) {
            charset = argv[2];
            infile = argv[3];
            outfile = argv[4];
        } else {
            infile = argv[2];
            outfile = argv[3];
        }

        bool charsetSet = false;
        UnicodeConvertStream stream;
        if (charset) {
            charsetSet = stream.SetCharset(charset);
            if (!charsetSet)
                printf("Unsupported charset: %s, will set to %s\n", charset, DEFAULT_CHARSET);
        }
        if (!charsetSet)
            stream.SetCharset(DEFAULT_CHARSET);

        inF = fopen(infile, "rb");
        outF = fopen(outfile, "wb");
        if (!inF || !outF) {
            printf("Can't open file\n");
            return 1;
        }

        UChar inBuf[1024];
        int n;
        while ((n = fread(inBuf, sizeof(UChar), 1024, inF)) > 0) {
            stream.UnicodeInput(inBuf, n, false);
        }

        fclose(inF);
        fclose(outF);
    }
    return 0;
}

