#include <stdio.h>
#include <string.h>
#include <time.h>

int ParseDateTime(const char* datetime, time_t* tt) {
    struct tm tm;
    memset(&tm, 0, sizeof(tm));
    if (0 == strptime(datetime, "%Y-%m-%d %H:%M:%S", &tm)) {
        return -1;
    }
    *tt = mktime(&tm);
    return 0;
}

int main(int argc, char* argv[]) {
    time_t tt;

    if (argc < 2) {
        printf("Usage:\n");
        printf("    %s [YYYY-MM-DD HH:MM:SS]\n", argv[0]);
        return 0;
    }

    if (0 == ParseDateTime(argv[1], &tt))
        printf("%d\n", tt);
    else
        printf("Parse failed\n");
    return 0;
}

