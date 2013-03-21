#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>

void Log2(const char* buf, size_t len) {
    write(1, buf, len);
}

void Log(const char* prefix, const char *format, ...) {
    const size_t BUF_LEN = 1024;
    char buf[BUF_LEN];

    size_t prefixLen = strlen(prefix);
    snprintf(buf, BUF_LEN, "%s", prefix);

    va_list ap;
    va_start(ap, format);
    vsnprintf(buf + prefixLen, BUF_LEN - prefixLen, format, ap);
    va_end(ap);

    size_t curLen = strlen(buf);
    snprintf(buf + curLen, BUF_LEN - curLen, "%s", "\n");

    Log2(buf, strlen(buf));
}

int main() {
    Log("[DEBUG] ", "%s %d %c %lf", "hello", 100, 'a', 1.23);
    short n = 300;
    Log("[INFO] ", "%d %d taco", 1000, n);
    return 0;
}

