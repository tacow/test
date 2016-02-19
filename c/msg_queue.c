#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define SEND_THREAD_NUM 10
#define SEND_TIMES      100000

#define RECV_THREAD_NUM 5
#define RECV_TIMES      (SEND_TIMES * SEND_THREAD_NUM / RECV_THREAD_NUM)

struct msgbuf {
    long mtype;
    long tid;
    long n;
};

int mq = 0;

void* SendThread(void* arg) {
    long tid;
    long i;

    tid = (long)arg;
    printf("Sender %ld start\n", tid);

    for(i = 0; i < SEND_TIMES; ++i) {
        struct msgbuf mb;
        mb.mtype = 1;
        mb.tid = tid;
        mb.n = i;
        if (msgsnd(mq, &mb, sizeof(long) * 2, 0) == -1)
            perror("Message send");
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
        struct msgbuf mb;
        if (msgrcv(mq, &mb, sizeof(long) * 2, 0, 0) == -1)
            perror("Message receive");
        fprintf(f, "%ld %ld\n", mb.tid, mb.n);
    }

    printf("Receiver %ld finish\n", tid);
    fclose(f);
    return NULL;
}

int main() {
    long i;
    pthread_t sendThreads[SEND_THREAD_NUM];
    pthread_t recvThreads[RECV_THREAD_NUM];

    mq = msgget(IPC_PRIVATE, 0600);
    if (mq == -1) {
        perror("Create message queue");
        return 0;
    }

    for(i = 0; i < RECV_THREAD_NUM; ++i)
        pthread_create(&recvThreads[i], NULL, RecvThread, (void*)i);

    for(i = 0; i < SEND_THREAD_NUM; ++i)
        pthread_create(&sendThreads[i], NULL, SendThread, (void*)i);

    for(i = 0; i < RECV_THREAD_NUM; ++i)
        pthread_join(recvThreads[i], NULL);

    for(i = 0; i < SEND_THREAD_NUM; ++i)
        pthread_join(sendThreads[i], NULL);

    if (msgctl(mq, IPC_RMID, NULL) == -1) {
        perror("Delete message queue");
        return 0;
    }
}

