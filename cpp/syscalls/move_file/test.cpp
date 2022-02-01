#include <stdio.h>
#include "movefile.h"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage:\n");
        printf("    %s [src file] [dst file]\n", argv[0]);
        return 0;
    }

    const char* srcFile = argv[1];
    const char* dstFile = argv[2];
    
    string errMsg;
    if (!MoveFile(srcFile, dstFile, errMsg))
    {
        printf("Move failed: %s\n", errMsg.c_str());
    }

    return 0;
}

