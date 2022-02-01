#include <stdio.h>
#include "copyfile.h"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage:\n");
        printf("    %s [src file] [dst file]\n", argv[0]);
        printf("    %s [src file] [dst file] [offset]\n", argv[0]);
        printf("    %s [src file] [dst file] [offset] [length]\n", argv[0]);
        return 0;
    }

    const char* srcFile = argv[1];
    const char* dstFile = argv[2];
    off_t offset = 0;
    size_t length = 0;
    if (argc >= 4)
    {
        offset = (off_t)strtol(argv[3], NULL, 10);
    }
    if (argc >= 5)
    {
        length = (size_t)strtoul(argv[4], NULL, 10);
    }
    else
    {
        struct stat fileStat;
        int res = stat(srcFile, &fileStat);
        if (res == -1)
        {
            printf("Can't stat source file\n");
            return -1;
        }
        length = (size_t)fileStat.st_size - (size_t)offset;
    }
    
    string errMsg;
    if (!CopyFile(srcFile, dstFile, offset, length, errMsg, 0600))
    {
        printf("Copy failed: %s\n", errMsg.c_str());
    }

    return 0;
}

