#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc < 4) {
        printf("Usage:\n");
        printf("    %s [str1] [str2] [n]\n", argv[0]);
        return 0;
    }

    const char* str1 = argv[1];
    const char* str2 = argv[2];
    size_t n = (size_t)atoi(argv[3]);

    int res = strncmp(str1, str2, n);
    printf("result=%d\n", res);
    return 0;
}

