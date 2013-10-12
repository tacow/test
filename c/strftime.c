#include <stdio.h>
#include <time.h>

int main(int argc, char* argv[]) {
    time_t tt;
    struct tm tmLocal;
    char timeStr[256];

    if (argc >= 2)
        tt = atoi(argv[1]);
    else
        tt = time(NULL);
    localtime_r(&tt, &tmLocal);
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &tmLocal);
    printf("%s\n", timeStr);
    return 0;
}

