#include <unicode/ucnv.h>

#define TO_UNICODE_BUF_LEN 1024
#define FROM_UNICODE_BUF_LEN 4096

class UnicodeConverter {
public:
    UnicodeConverter(int toUnicodeBufLen = TO_UNICODE_BUF_LEN,
                     int fromUnicodeBufLen = FROM_UNICODE_BUF_LEN);
    ~UnicodeConverter();

    bool SetCharset(const char* charset);

    UChar* ConvertToUnicode(const char* data, int len, int* destLen);
    char* ConvertFromUnicode(const UChar* data, int len, int* destLen);

private:
    UConverter* converter_;
    UChar* toUnicodeBuf_;
    int toUnicodeBufLen_;
    char* fromUnicodeBuf_;
    int fromUnicodeBufLen_;
};

