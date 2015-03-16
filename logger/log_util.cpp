#include "log_util.h"
#include <stdarg.h>
#include <pthread.h>

static pthread_key_t g_key;

static void DeleteMsgBuf(void* ptr) {
    delete[] (char*)ptr;
}

void InitLogUtil() {
    pthread_key_create(&g_key, DeleteMsgBuf);
}

void DestroyLogUtil() {
    pthread_key_delete(g_key);
}

bool VLogReal(MsgLogger& logger, int level, const char* sourceFile, int lineNum, const char* format, ...) {
    if (level > logger.GetMaxLevel())
        return true;

    char* msg = (char*)pthread_getspecific(g_key);
    if (!msg) {
        msg = new char[MSG_BUF_SIZE];
        pthread_setspecific(g_key, msg);
    }

    va_list ap;
    va_start(ap, format);
    vsnprintf(msg, MSG_BUF_SIZE, format, ap);
    va_end(ap);

    if (sourceFile)
        return logger.Log(msg, level, sourceFile, lineNum);
    else
        return logger.Log(msg, level);
}

