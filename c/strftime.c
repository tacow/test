#include <stdio.h>
#include <time.h>

int main(int argc, char* argv[]) {
    time_t ttNow;
    struct tm tmLocal;
    char timeStr[256];
    ttNow = time(NULL);
    localtime_r(&ttNow, &tmLocal);
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &tmLocal);
    printf("%s\n", timeStr);
    return 0;
}

