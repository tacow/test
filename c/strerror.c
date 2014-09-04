#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("%s [errno]\n", argv[0]);
        return 0;
    }

    int errCode = atoi(argv[1]);
    char* errMsg = strerror(errCode);
    printf("%s\n", errMsg);
    return 0;
}

