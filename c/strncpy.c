#include <stdio.h>
#include <string.h>

void Display(const char* str, size_t n) {
    size_t i;
    for (i = 0; i < n; ++i)
        printf("[%c] %02x ", str[i], str[i]);
    printf("\n");
}

int main(int argc, char* argv[]) {
    char buf[] = "aa";
    char buf2[5] = "aaa";
    char buf3[8] = "aaa";
    Display(buf, sizeof(buf));
    Display(buf2, sizeof(buf2));
    Display(buf3, sizeof(buf3));

    strncpy(buf, "hello", sizeof(buf));
    strncpy(buf2, "hello", sizeof(buf2));
    strncpy(buf3, "hello", sizeof(buf3));

    Display(buf, sizeof(buf));
    Display(buf2, sizeof(buf2));
    Display(buf3, sizeof(buf3));
    return 0;
}

