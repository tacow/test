#include <stdio.h>
#include <wchar.h>
#include <string.h>

#include <unicode/ucnv.h>

#include <string>
#include <algorithm>

#define DEFAULT_CHARSET "UTF-8"
#define TO_UNICODE_BUF_LEN 1024
#define FROM_UNICODE_BUF_LEN 4096

class UnicodeConverter {
  private:
    UConverter* converter_;
    UChar* toUnicodeBuf_;
    int toUnicodeBufLen_;
    char* fromUnicodeBuf_;
    int fromUnicodeBufLen_;

  public:
    UnicodeConverter(int toUnicodeBufLen = TO_UNICODE_BUF_LEN,
                     int fromUnicodeBufLen = FROM_UNICODE_BUF_LEN) {
      converter_ = NULL;
      toUnicodeBufLen_ = toUnicodeBufLen;
      toUnicodeBuf_ = new UChar[toUnicodeBufLen];
      fromUnicodeBufLen_ = fromUnicodeBufLen;
      fromUnicodeBuf_ = new char[fromUnicodeBufLen];
    }

    ~UnicodeConverter() {
      if (converter_)
        ucnv_close(converter_);
      delete[] toUnicodeBuf_;
      delete[] fromUnicodeBuf_;
    }

    bool Init(const char* charset = NULL) {
      if (!charset)
        charset = DEFAULT_CHARSET;
      return SetCharset(charset);
    }

    UChar* ConvertToUnicode(const char* data, int len, int* destLen) {
      UErrorCode err = U_ZERO_ERROR;
      *destLen = ucnv_toUChars(converter_, toUnicodeBuf_, toUnicodeBufLen_, data, len, &err);
      if (*destLen > toUnicodeBufLen_)
        *destLen = toUnicodeBufLen_;
      return toUnicodeBuf_;
    }

    char* ConvertFromUnicode(const UChar* data, int len, int* destLen) {
      UErrorCode err = U_ZERO_ERROR;
      *destLen = ucnv_fromUChars(converter_, fromUnicodeBuf_, fromUnicodeBufLen_, data, len, &err);
      if (*destLen > fromUnicodeBufLen_)
        *destLen = fromUnicodeBufLen_;
      return fromUnicodeBuf_;
    }

  private:
    bool SetCharset(const char* charset) {
      if (converter_) {
        ucnv_close(converter_);
        converter_ = NULL;
      }
      UErrorCode err = U_ZERO_ERROR;
      converter_ = ucnv_open(charset, &err);
      return (converter_ != NULL);
    }
};

FILE* inF = NULL;
FILE* outF = NULL;

void WriteUChar(const UChar* str, int len) {
  fwrite(str, sizeof(UChar), len, outF);
}

void WriteData(const char* str, int len) {
  fwrite(str, sizeof(char), len, outF);
}

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
    inF = fopen(infile, "rb");
    outF = fopen(outfile, "wb");
    if (!inF || !outF) {
      printf("Can't open file\n");
      return 1;
    }

    UChar bom = 0xfeff;
    WriteUChar(&bom, 1);

    UnicodeConverter converter;
    converter.Init(charset);
    char inBuf[4096];
    int n = fread(inBuf, 1, 4096, inF);
    int convertLen;
    UChar* buf = converter.ConvertToUnicode(inBuf, n, &convertLen);
    WriteUChar(buf, convertLen);

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
    inF = fopen(infile, "rb");
    outF = fopen(outfile, "wb");
    if (!inF || !outF) {
      printf("Can't open file\n");
      return 1;
    }

    UnicodeConverter converter;
    converter.Init(charset);
    UChar inBuf[1024];
    int n = fread(inBuf, sizeof(UChar), 1024, inF);
    int convertLen;
    char* buf = converter.ConvertFromUnicode(inBuf, n, &convertLen);
    WriteData(buf, convertLen);

    fclose(inF);
    fclose(outF);
  }
  return 0;
}

