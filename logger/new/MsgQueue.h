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

    // 遍历消息队列
    // msg为获得的消息（输出参数）
    // iter为当前迭代器，第一次传入NULL，以后每次GetNext会被更新（输入输出参数）
    // 返回true代表还有下一个消息，返回false代表遍历结束
    // 注：本函数没有加锁
    bool GetNext(void*& msg, void*& iter);

private:
    pthread_mutex_t m_mutex;
    sem_t m_sem;

    MsgQueueNode m_queue;
    MemPool m_pool;
};

#endif

