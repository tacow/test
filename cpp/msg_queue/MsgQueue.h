#ifndef MSG_QUEUE_H
#define MSG_QUEUE_H

#include <pthread.h>
#include <semaphore.h>
#include "MemPool.h"

struct MsgQueueNode {
    MsgQueueNode* next;
    MsgQueueNode* prev;
    void* msg;
};

class MsgQueue {
public:
    MsgQueue();
    ~MsgQueue();

    void Push(void* msg);
    void* Pop();

private:
    pthread_mutex_t m_mutex;
    sem_t m_sem;

    MsgQueueNode m_queue;
    MemPool m_pool;
};

#endif

