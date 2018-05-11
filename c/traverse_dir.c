#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

void PathAppendEntry(char* entryPathBuf, int entryPathLen, const char* basePath, const char* entryName) {
    size_t basePathLen = strlen(basePath);
    if (basePathLen == 0) {
        snprintf(entryPathBuf, entryPathLen, "%s", entryName);
    } else {
        if (basePath[basePathLen - 1] == '/')
            snprintf(entryPathBuf, entryPathLen, "%s%s", basePath, entryName);
        else
            snprintf(entryPathBuf, entryPathLen, "%s/%s", basePath, entryName);
    }
}

void TraverseDir(const char* path, const char* relativePath) {
    DIR* dir = opendir(path);
    if (!dir)
        return;

    struct dirent entryBuf;
    struct dirent* entry = NULL;
    while (0 == readdir_r(dir, &entryBuf, &entry) && entry) {
        const char* entryName = entry->d_name;
        if (strcmp(entryName, ".") == 0 || strcmp(entryName, "..") == 0)
            continue;

        char entryPath[1024];
        PathAppendEntry(entryPath, 1024, path, entryName);

        char relativeEntryPath[1024];
        PathAppendEntry(relativeEntryPath, 1024, relativePath, entryName);

        struct stat statBuf;
        if (0 != lstat(entryPath, &statBuf))
            continue;
        if (S_ISREG(statBuf.st_mode))
            printf("%s | %s\n", entryPath, relativeEntryPath);
        else if (S_ISDIR(statBuf.st_mode))
            TraverseDir(entryPath, relativeEntryPath);
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
    TraverseDir(path, "");
    return 0;
}

