#include <stdio.h>
#include "mq.h"

#define SEND_THREAD_NUM 10
#define SEND_TIMES      100000

#define RECV_THREAD_NUM 5
#define RECV_TIMES      (SEND_TIMES * SEND_THREAD_NUM / RECV_THREAD_NUM)

mq_t mq;

void* SendThread(void* arg) {
    long tid;
    long i;

    tid = (long)arg;
    printf("Sender %ld start\n", tid);

    for(i = 0; i < SEND_TIMES; ++i) {
        long msg = tid * SEND_TIMES + i;
        mq_push(&mq, (void*)msg);
    }

    printf("Sender %ld finish\n", tid);
    return NULL;
}

void* RecvThread(void* arg) {
    long tid;
    long i;
    char filename[256];
    FILE* f;

    tid = (long)arg;
    snprintf(filename, 256, "recv_%ld.txt", tid);
    f = fopen(filename, "w");
    printf("Receiver %ld start\n", tid);

    for(i = 0; i < RECV_TIMES; ++i) {
        long msg = (long)mq_pop(&mq);
        fprintf(f, "%ld\n", msg);
    }

    printf("Receiver %ld finish\n", tid);
    fclose(f);
    return NULL;
}

int main() {
    long i;
    pthread_t sendThreads[SEND_THREAD_NUM];
    pthread_t recvThreads[RECV_THREAD_NUM];

    mq_init(&mq);

    for(i = 0; i < RECV_THREAD_NUM; ++i)
        pthread_create(&recvThreads[i], NULL, RecvThread, (void*)i);

    for(i = 0; i < SEND_THREAD_NUM; ++i)
        pthread_create(&sendThreads[i], NULL, SendThread, (void*)i);

    for(i = 0; i < RECV_THREAD_NUM; ++i)
        pthread_join(recvThreads[i], NULL);

    for(i = 0; i < SEND_THREAD_NUM; ++i)
        pthread_join(sendThreads[i], NULL);

    mq_cleanup(&mq);
    return 0;
}

