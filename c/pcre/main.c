#include <stdio.h>
#include <string.h>
#include <pcre.h>

int Replace(const char* src, const char* pattern, const char* replacement, char* dest) {
    pcre* re;
    const char* errptr = NULL;
    int erroffset = -1;
    int srcLen = 0;
    int startoffset = 0;
    int ovector[30];
    int n = 0;

    re = pcre_compile(pattern, 0, &errptr, &erroffset, NULL);
    if (!re) {
        printf("Compile failed at %d: %s\n", erroffset, errptr);
        return 1;
    }

    srcLen = strlen(src);
    dest[0] = '\0';
    while(1) {
        n = pcre_exec(re, NULL, src, srcLen, startoffset, 0, ovector, 30);
        if (n < 0) {
            strcat(dest, src + startoffset);
            break;
        }
        strncat(dest, src + startoffset, ovector[0] - startoffset);
        strcat(dest, replacement);
        startoffset = ovector[1];
    }

    pcre_free(re);
    return 0;
}

int main(int argc, char* argv[]) {
    char* source = NULL;
    char* pattern = NULL;
    char* replacement = NULL;
    char destination[4096];

    if (argc < 4) {
        printf("Usage: %s [source] [pattern] [replacement]\n", argv[0]);
        return 0;
    }
    source = argv[1];
    pattern = argv[2];
    replacement = argv[3];

    if (0 == Replace(source, pattern, replacement, destination)) {
        printf("%s\n", destination);
    }
    return 0;
}

