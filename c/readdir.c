#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage:\n");
        printf("    %s [dir]\n", argv[0]);
        return 0;
    }

    const char* path = argv[1];
    DIR* dir = opendir(path);
    if (!dir)
        return 0;

    struct dirent entry;
    struct dirent* result = NULL;
    while (0 == readdir_r(dir, &entry, &result) && result)
        printf("%s\n", result->d_name);
    closedir(dir);
    return 0;
}

