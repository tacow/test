#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#define LOG_FILE_PATH "/tmp/mylog_%Y%m%d_%H%M%S.log"

static FILE* logF = NULL;

void MyLog(const char* format, ...) {
    if (!logF) {
        time_t ttNow;
        time(&ttNow);

        struct tm tmLocal;
        localtime_r(&ttNow, &tmLocal);

        char logFilePath[256];
        strftime(logFilePath, sizeof(logFilePath), LOG_FILE_PATH, &tmLocal);
        logF = fopen(logFilePath, "w");
    }

    time_t ttNow;
    time(&ttNow);

    struct tm tmLocal;
    localtime_r(&ttNow, &tmLocal);

    char timeStr[256];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &tmLocal);
    fprintf(logF, "[%s] ", timeStr);

    va_list ap;
    va_start(ap, format);
    vfprintf(logF, format, ap);
    va_end(ap);

    fprintf(logF, "\n");
    fflush(logF);
}

