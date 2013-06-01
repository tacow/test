#include <stdio.h>
#include <string.h>

void Find(const char* str) {
    char line[4096];
    while(fgets(line, 4096, stdin)) {
        if (strstr(line, str))
            printf("%s", line);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage:\n");
        printf("    %s [substr]\n", argv[0]);
        return 0;
    }

    Find(argv[1]);
    return 0;
}

