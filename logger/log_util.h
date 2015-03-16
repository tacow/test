#ifndef LOG_UTIL_H
#define LOG_UTIL_H

#include "logger.h"

#ifndef MSG_BUF_SIZE
#define MSG_BUF_SIZE (1024 * 1024)
#endif

void InitLogUtil();
void DestroyLogUtil();
bool VLogReal(MsgLogger& logger, int level, const char* sourceFile, int lineNum, const char* format, ...);

#ifdef LOG_SOURCE
#define VLog(logger, level, format, ...) \
    VLogReal(logger, level, __FILE__, __LINE__, format, ##__VA_ARGS__)
#else
#define VLog(logger, level, format, ...) \
    VLogReal(logger, level, NULL, 0, format, ##__VA_ARGS__)
#endif

#endif

