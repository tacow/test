#include <stdio.h>
#include <stdarg.h>

void test(int argNum, ...) {
    int i;
    va_list ap;
    va_start(ap, argNum);
    for(i = 0; i < argNum; ++i) {
        char* str = va_arg(ap, char*); 
        printf("%s\n", str);
    }
    va_end(ap);
}

void test2(const char* format, ...) {
    va_list ap;
    va_start(ap, format);
    vprintf(format, ap);
    va_end(ap);
}

int main(int argc, char* argv[]) {
    test(3, "abc", "def", "ghi");
    test2("hello %d %s\n", 100, "aaa");
    return 0;
}

