// g++ -g -o UnicodeConvertStream UnicodeConvertStream.cpp -licui18n

#include <stdio.h>
#include <wchar.h>
#include <string.h>

#include <unicode/ucnv.h>
#include <unicode/ucsdet.h>

#include <string>
#include <algorithm>

#define DEFAULT_CHARSET "UTF-8"
#define DETECT_MAX_LEN 131072
#define DETECT_MIN_NON_ASCII_LEN 6
#define DETECT_MIN_SCORE 10
#define TO_UNICODE_BUF_LEN 1024
#define FROM_UNICODE_BUF_LEN 4096

void WriteUChar(const UChar* str, int len);
void WriteData(const char* str, int len);

#define IsLeadSurrogate(u)  ((u) >= 0xD800 && (u) <= 0xDBFF)
#define IsTrailSurrogate(u) ((u) >= 0xDC00 && (u) <= 0xDFFF)
#define AssembleSurrogatePair(u1,u2) (((((UChar32)(u1) & 0x03FF) << 10) | ((UChar32)(u2) & 0x03FF)) + (UChar32)0x10000)

class UnicodeConvertStream {
  private:
    UConverter* converter_;
    UChar* toUnicodeBuf_;
    char* fromUnicodeBuf_;

    bool detected_;
    std::string currentCharset_;
    char* detectBuf_;
    int   detectLen_;

  public:
    UnicodeConvertStream() {
      converter_ = NULL;
      toUnicodeBuf_ = new UChar[TO_UNICODE_BUF_LEN];
      fromUnicodeBuf_ = new char[FROM_UNICODE_BUF_LEN];

      detected_ = false;
      detectBuf_ = NULL;
      detectLen_ = 0;
    }

    ~UnicodeConvertStream() {
      if (converter_)
        ucnv_close(converter_);
      delete[] toUnicodeBuf_;
      delete[] fromUnicodeBuf_;

      if (detectBuf_)
        delete[] detectBuf_;
    }

    void Init(const char* charset = NULL) {
      if (charset) {
        if (SetCharset(charset)) {
          detected_ = true;
          OnDetecedCharset(charset);
          return;
        }
      }

      detected_ = false;
      detectBuf_ = new char[DETECT_MAX_LEN];
      detectLen_ = 0;
    }

    void CodePageInput(const char* data, int len, bool flush) {
      if (!detected_) {
        int copyLen = std::min(len, DETECT_MAX_LEN - detectLen_);
        memcpy(detectBuf_ + detectLen_, data, copyLen);
        detectLen_ += copyLen;

        if (NeedDetect()) {
          std::string charset = DetectCharset();
          SetCharset(charset);
          detected_ = true;
          delete[] detectBuf_;
          detectBuf_ = NULL;
          detectLen_ = 0;

          OnDetecedCharset(charset);
        }
      }
      if (!converter_)
        SetCharset(DEFAULT_CHARSET);

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

    void OnUnicodeOutput(UChar* str, int len) {
      int i = 0;
      while (i < len) {
        UChar32 u32 = 0;
        int chLen = 1;
        bool valid = true;
        if (IsLeadSurrogate(str[i])) {
          if (i + 1 < len && IsTrailSurrogate(str[i + 1])) {
            u32 = AssembleSurrogatePair(str[i], str[i + 1]);
            chLen = 2;
          } else {
            valid = false;
          }
        } else if (IsTrailSurrogate(str[i])) {
          valid = false;
        } else {
          u32 = str[i];
        }
        if (valid) {
          int width = wcwidth(u32);
          if (width > 0)
            OnUniCharOutput(&str[i], chLen, width);
        }
        i += chLen;
      }
    }

    void OnUniCharOutput(UChar* ch, int len, int width) {
      WriteUChar(ch, len);
    }

    void UnicodeInput(const UChar* data, int len, bool flush) {
      if (!converter_)
        SetCharset(DEFAULT_CHARSET);

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

    void OnCodePageOutput(char* str, int len) {
      WriteData(str, len);
    }

    void OnDetecedCharset(const std::string& charset) {
      printf("\n* Detected charset: %s\n", charset.c_str());
    }

  private:
    bool NeedDetect() {
      if (detectLen_ == DETECT_MAX_LEN)
        return true;
      int nonASCIIByteCount = 0;
      for (int i = 0; i < detectLen_; ++i) {
        if ((detectBuf_[i] & 0x80)) {
          ++nonASCIIByteCount;
        }
        if (nonASCIIByteCount > DETECT_MIN_NON_ASCII_LEN)
          return true;
      }
      return false;
    }

    std::string DetectCharset() {
      std::string charset;
      UErrorCode err = U_ZERO_ERROR;
      UCharsetDetector* detector = ucsdet_open(&err);
      ucsdet_setText(detector, detectBuf_, detectLen_, &err);
      const UCharsetMatch* match = ucsdet_detect(detector, &err);
      if (match) {
        int score = ucsdet_getConfidence(match, &err);
        if (score >= DETECT_MIN_SCORE)
          charset = ucsdet_getName(match, &err);
      }
      if (charset.empty())
        charset = DEFAULT_CHARSET;
      ucsdet_close(detector);
      return charset;
    }

    bool SetCharset(const std::string& charset) {
      if (converter_ && charset == currentCharset_)
        return true;

      if (converter_) {
        ucnv_close(converter_);
        converter_ = NULL;
      }
      UErrorCode err = U_ZERO_ERROR;
      converter_ = ucnv_open(charset.c_str(), &err);
      if (converter_) {
        currentCharset_ = charset;
        return true;
      } else {    // Fallback
        converter_ = ucnv_open(DEFAULT_CHARSET, &err);
        currentCharset_ = DEFAULT_CHARSET;
        return false;
      }
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

    char inBuf[4096];
    int n;
    UnicodeConvertStream stream;
    stream.Init(charset);
    while ((n = fread(inBuf, 1, 4096, inF)) > 0) {
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
    inF = fopen(infile, "rb");
    outF = fopen(outfile, "wb");
    if (!inF || !outF) {
      printf("Can't open file\n");
      return 1;
    }

    UChar inBuf[1024];
    int n;
    UnicodeConvertStream stream;
    stream.Init(charset);
    while ((n = fread(inBuf, sizeof(UChar), 1024, inF)) > 0) {
      stream.UnicodeInput(inBuf, n, false);
    }

    fclose(inF);
    fclose(outF);
  }
  return 0;
}

