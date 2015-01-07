#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage:\n");
        printf("    %s [tokens split by \",\"]\n", argv[0]);
        return 0;
    }

    char buf[1024];
    snprintf(buf, 1024, "%s", argv[1]);

    char* ptr;
    char* token = strtok_r(buf, ",", &ptr);
    while(token) {
        printf("%s\n", token);
        token = strtok_r(NULL, ",", &ptr);
    }
    return 0;
}

