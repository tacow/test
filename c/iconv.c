#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iconv.h>

#define HEAD_N 10
#define SYS_CHARSET "GB18030"

size_t LimitCharNum(char* str, size_t len, size_t limitNum, const char* charset) {
    iconv_t cd;
    char* oriStr = str;
    char* unicodePtr;
    char* oriUnicodePtr;
    size_t unicodeLen;

    cd = iconv_open("UTF-16LE", charset);
    if (cd == (iconv_t)-1)
        return (size_t)-1;

    unicodeLen = limitNum * 2;
    unicodePtr = (char*)malloc(unicodeLen);
    oriUnicodePtr = unicodePtr;
    iconv(cd, &str, &len, &unicodePtr, &unicodeLen);
    free(oriUnicodePtr);
    iconv_close(cd);

    return str - oriStr;
}

int main(int argc, char* argv[]) {
    char line[4096];
    size_t lineLen;
    size_t newLen;

    fgets(line, 4096, stdin);
    lineLen = strlen(line);

    newLen = LimitCharNum(line, lineLen, HEAD_N, SYS_CHARSET);
    if ((size_t)-1 == newLen) {
        printf("Failed\n");
        return 0;
    }
    line[newLen] = '\0';
    puts(line);
    return 0;
}

