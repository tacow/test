// g++ -g -o UnicodeConverterTest UnicodeConverterTest.cpp UnicodeConverter.cpp -licui18n

#include <stdio.h>
#include <string.h>
#include "UnicodeConverter.h"

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
    const char* charset = "UTF-8";
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
    converter.SetCharset(charset);
    char inBuf[4096];
    int n = fread(inBuf, 1, 4096, inF);
    int convertLen;
    UChar* buf = converter.ConvertToUnicode(inBuf, n, &convertLen);
    WriteUChar(buf, convertLen);

    fclose(inF);
    fclose(outF);
  } else {
    const char* charset = "UTF-8";
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
    converter.SetCharset(charset);
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

