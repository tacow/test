#include <stdio.h>
#include "FTLogger.h"

#define THREAD_NUM  10
#define LOG_TIMES   1000

FTLogger logger;

void* ThreadFunc(void* arg) {
    long tid = (long)arg;

    for(long i = 0; i < LOG_TIMES; ++i) {
        VLog(logger, LOG_INFO, "%ld,%ld", tid, i);
    }

    printf("Thread %ld finish\n", tid);
    return NULL;
}

int main() {
    logger.Init("test", ".");

    pthread_t threads[THREAD_NUM];
    for(long i = 0; i < THREAD_NUM; ++i)
        pthread_create(&threads[i], NULL, ThreadFunc, (void*)i);

    for(long i = 0; i < THREAD_NUM; ++i)
        pthread_join(threads[i], NULL);

    logger.Close();
    return 0;
}

