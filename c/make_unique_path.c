#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int PathExist(const char* path) {
    struct stat statBuf;
    if (0 == lstat(path, &statBuf)) {
        return 1;       // Exist
    } else {
        if (errno == ENOENT)
            return 0;   // Not exist
        else
            return -1;  // Error
    }
}

int MakeUniquePath(const char* path, char* uniquePathBuf, int uniquePathLen) {
    int res = PathExist(path);
    if (res == 0) {
        snprintf(uniquePathBuf, uniquePathLen, "%s", path);
        return 1;
    } else if (res == -1) {
        return 0;
    }

    int index = 2;
    while (1) {
        snprintf(uniquePathBuf, uniquePathLen, "%s.%d", path, index);
        res = PathExist(uniquePathBuf);
        if (res == 0)
            return 1;
        else if (res == -1)
            return 0;
        ++index;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage:\n");
        printf("    %s [path]\n", argv[0]);
        return 0;
    }

    const char* path = argv[1];
    char uniquePath[256];
    if (MakeUniquePath(path, uniquePath, 256))
        printf("%s\n", uniquePath);
    else
        printf("Illegal path\n");
    return 0;
}

