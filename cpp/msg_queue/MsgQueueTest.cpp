#include <stdio.h>
#include "MsgQueue.h"

#define SEND_THREAD_NUM 10
#define SEND_TIMES      1000000

#define RECV_THREAD_NUM 5
#define RECV_TIMES      (SEND_TIMES * SEND_THREAD_NUM / RECV_THREAD_NUM)

MsgQueue mq;

void* SendThread(void* arg) {
    long tid = (long)arg;

    char filename[256];
    snprintf(filename, 256, "send_%ld.txt", tid);
    FILE* f = fopen(filename, "w");
    printf("Sender %ld start\n", tid);

    for(long i = 0; i < SEND_TIMES; ++i) {
        long msg = tid * SEND_TIMES + i;
        mq.Push((void*)msg);
        fprintf(f, "%ld\n", msg);
    }

    printf("Sender %ld finish\n", tid);
    fclose(f);
    return NULL;
}

void* RecvThread(void* arg) {
    long tid = (long)arg;

    char filename[256];
    snprintf(filename, 256, "recv_%ld.txt", tid);
    FILE* f = fopen(filename, "w");
    printf("Receiver %ld start\n", tid);

    for(long i = 0; i < RECV_TIMES; ++i) {
        long msg = (long)mq.Pop();
        fprintf(f, "%ld\n", msg);
    }

    printf("Receiver %ld finish\n", tid);
    fclose(f);
    return NULL;
}

int main() {
    pthread_t sendThreads[SEND_THREAD_NUM];
    pthread_t recvThreads[RECV_THREAD_NUM];

    for(long i = 0; i < RECV_THREAD_NUM; ++i)
        pthread_create(&recvThreads[i], NULL, RecvThread, (void*)i);

    for(long i = 0; i < SEND_THREAD_NUM; ++i)
        pthread_create(&sendThreads[i], NULL, SendThread, (void*)i);

    for(long i = 0; i < RECV_THREAD_NUM; ++i)
        pthread_join(recvThreads[i], NULL);

    for(long i = 0; i < SEND_THREAD_NUM; ++i)
        pthread_join(sendThreads[i], NULL);

    return 0;
}

