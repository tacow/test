#include <stdio.h>
#include <string.h>
#include <time.h>

int ParseDate(const char* date, time_t* tt) {
    struct tm tm;
    memset(&tm, 0, sizeof(tm));
    if (0 == strptime(date, "%Y%m%d", &tm)) {
        return -1;
    }
    *tt = mktime(&tm);
    return 0;
}

int main(int argc, char* argv[]) {
    time_t ttStart;
    time_t ttEnd;
    time_t date;

    if (argc < 3) {
        printf("Usage:\n");
        printf("    %s [YYYYMMDD] [YYYYMMDD]\n", argv[0]);
        return 0;
    }

    if (-1 == ParseDate(argv[1], &ttStart)) {
        printf("Parse start date failed\n");
        return 0;
    }

    if (-1 == ParseDate(argv[2], &ttEnd)) {
        printf("Parse end date failed\n");
        return 0;
    }

    if (ttStart > ttEnd) {
        printf("Start date should be earlier than end date\n");
        return 0;
    }

    for(date = ttStart; date <= ttEnd; date += 24 * 3600) {
        struct tm tmLocal;
        char dateStr[256];

        localtime_r(&date, &tmLocal);
        strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &tmLocal);
        printf("%s\n", dateStr);
    }

    return 0;
}


