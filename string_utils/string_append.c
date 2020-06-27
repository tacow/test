#include <stdio.h>
#include <string.h>
#include <stdarg.h>

void string_append(char* buf, size_t n, const char* fmt, ...) {
    size_t cur_len = strnlen(buf, n);
    if (cur_len + 1 >= n)
        return;

    va_list args;
    va_start(args, fmt);
    vsnprintf(buf + cur_len, n - cur_len, fmt, args);
    va_end(args);
}

