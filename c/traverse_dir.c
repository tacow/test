#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

void TraverseDir(const char* path) {
    size_t pathLen = strlen(path);
    if (pathLen == 0)
        return;

    DIR* dir = opendir(path);
    if (!dir)
        return;

    struct dirent entryBuf;
    struct dirent* entry = NULL;
    while (0 == readdir_r(dir, &entryBuf, &entry) && entry) {
        const char* entryName = entry->d_name;
        if (strcmp(entryName, ".") == 0 || strcmp(entryName, "..") == 0)
            continue;

        char entryPath[256];
        if (path[pathLen - 1] == '/')
            snprintf(entryPath, 256, "%s%s", path, entryName);
        else
            snprintf(entryPath, 256, "%s/%s", path, entryName);

        struct stat statBuf;
        if (0 != lstat(entryPath, &statBuf))
            continue;
        if (S_ISREG(statBuf.st_mode))
            printf("%s\n", entryPath);
        else if (S_ISDIR(statBuf.st_mode))
            TraverseDir(entryPath);
    }
    closedir(dir);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage:\n");
        printf("    %s [dir]\n", argv[0]);
        return 0;
    }

    const char* path = argv[1];
    TraverseDir(path);
    return 0;
}

