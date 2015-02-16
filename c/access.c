#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage:\n");
        printf("    %s [filename]\n", argv[0]);
        return 0;
    }

    const char* filename = argv[1];
    if (-1 == access(filename, F_OK))
        printf("Fail: %s(%d)\n", strerror(errno), errno);
    else
        printf("Ok\n");

    return 0;
}

