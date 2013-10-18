#include <stdio.h>
#include <string.h>
#include <errno.h>

int mkdir2(const char* dirname) {
    if (0 == mkdir(dirname, 0755)) {
        printf("ok\n");
        return 0;
    } else {
        printf("failed: %s(%d)\n", strerror(errno), errno);
        return -1;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage:\n");
        printf("    %s [dirname]\n", argv[0]);
        return 0;
    }
    mkdir2(argv[1]);
    return 0;
}

