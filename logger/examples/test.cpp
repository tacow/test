#include "logger.h"
#include "GbToUtf8Modifier.h"
#include "StrReplaceModifier.h"
#include <stdlib.h>

#define LOG_PATH            "./log/" 
#define LOG_THREADS         20
#define LOG_TIMES           200000

MsgLogger g_logger1;
MsgLogger g_logger2;

GbToUtf8Modifier g_gbToUtf8Modifier;
StrReplaceModifier g_strReplaceModifier;

int Rand(int bound) {
    return rand() % bound;
}

string GetRandStr(size_t len) {
    char buf[8192];
    for(size_t i = 0; i < len; ++i)
        buf[i] = 'a' + Rand(26);
    buf[len] = '\0';
    return buf;
}

void* LogThread(void* ptr) {
    for(int i = 0; i < LOG_TIMES; ++i) {
        string msg = GetRandStr(5) + "," + GetRandStr(9) + "," + GetRandStr(6);
        if (i % 100 == 0) {
            g_logger1.VLog(LOG_ERROR, "%d: %s", i, msg.c_str());
            g_logger2.VLog(LOG_WARNING, "%d: %s", i, msg.c_str());
        } else if (i % 3 == 0) {
            g_logger1.VLog(LOG_NOTICE, "%d: 中文测试", i);
            g_logger2.VLog(LOG_NOTICE, "%d: 测试中文", i);
        } else {
            g_logger1.VLog(LOG_INFO, "%d: %s", i, msg.c_str());
            g_logger2.VLog(LOG_INFO, "%d: %s", i, msg.c_str());
        }
    }
    return NULL;
}

int main() {
    g_logger1.Init("test1", LOG_PATH);
    g_logger1.SetPrintToScreen();
    g_logger1.SetSeperator(",");

    g_strReplaceModifier.Init(",", "<<comma>>");
    g_logger1.AddMsgModifier(&g_strReplaceModifier);

    g_logger2.Init("test2", LOG_PATH);

    g_gbToUtf8Modifier.Init();
    g_logger2.AddMsgModifier(&g_gbToUtf8Modifier);

    pthread_t logThreads[LOG_THREADS];

    for(int i = 0; i < LOG_THREADS; ++i)
        pthread_create(&logThreads[i], NULL, LogThread, (void*)i);

    for(int i = 0; i < LOG_THREADS; ++i)
        pthread_join(logThreads[i], NULL);

    g_logger1.Close();
    g_logger2.Close();
    return 0;
}

