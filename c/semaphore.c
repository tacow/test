#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define THREAD_COUNT    10
#define POST_COUNT      10

sem_t sem;

void RandomSleep(int msecs) {
    struct timespec ts;
    int nsecs = (rand() % msecs + 1) * 1000 * 1000;
    ts.tv_sec = nsecs / 1000000000;
    ts.tv_nsec = nsecs % 1000000000;
    nanosleep(&ts, NULL);
}

void* SemPosterThread(void* threadID) {
    int i;
    for (i = 0; i < POST_COUNT; ++i) {
        int semval = 0;
        RandomSleep(100);
        sem_post(&sem);
        sem_getvalue(&sem, &semval);
        printf("Poster %ld: i=%d, after sem_post, sem_getvalue(sem)=%d\n", (long)threadID, i, semval);
    }
}

void* SemWaiterThread(void* threadID) {
    int i;
    for (i = 0; i < POST_COUNT; ++i) {
        int semval = 0;
        RandomSleep(100);
        sem_getvalue(&sem, &semval);
        printf("Waiter %ld: i=%d, before sem_wait, sem_getvalue(sem)=%d\n", (long)threadID, i, semval);
        sem_wait(&sem);
    }
}

int main() {
    long i;
    pthread_t posterThreads[THREAD_COUNT];
    pthread_t waiterThreads[THREAD_COUNT];

    srand(time(NULL));
    sem_init(&sem, 0, 0);
    for (i = 0; i < THREAD_COUNT; ++i)
        pthread_create(&waiterThreads[i], NULL, SemWaiterThread, (void*)i);
    for (i = 0; i < THREAD_COUNT; ++i)
        pthread_create(&posterThreads[i], NULL, SemPosterThread, (void*)i);

    for (i = 0; i < THREAD_COUNT; ++i)
        pthread_join(waiterThreads[i], NULL);
    for (i = 0; i < THREAD_COUNT; ++i)
        pthread_join(posterThreads[i], NULL);
    return 0;
}

