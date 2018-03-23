#include "UnicodeConverter.h"
#include <unicode/ustdio.h>
#include <stdio.h>
#include <string.h>

UnicodeConverter g_conv;

const UChar* Input(int* ulen) {
    char buf[1024] = { '\0' };
    fgets(buf, 1024, stdin);
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n')
        --len;
    UChar* ustr = g_conv.ConvertToUnicode(buf, len, ulen);
    return ustr;
}

void Output(UChar* ustr, int ulen) {
    int len = 0;
    char* str = g_conv.ConvertFromUnicode(ustr, ulen, &len);
    fwrite(str, 1, len, stdout);
}

int main() {
    g_conv.SetCharset("UTF-8");

    int nameLen = 0;
    const UChar* name = Input(&nameLen);

    UChar buf[256];
    int len = u_snprintf_u(buf, 256, (const UChar*)L"你好%*S\n", nameLen, name);
    Output(buf, len);

    for (int i = 0; i <= 40; ++i) {
        double val = 2.5 * (double)i;
        len = u_snprintf_u(buf, 256, (const UChar*)L" %6.2lf  %-6.2lf\n", val, val);
        Output(buf, len);
    }
    return 0;
}

