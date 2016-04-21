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

    // ������Ϣ����
    // msgΪ��õ���Ϣ�����������
    // iterΪ��ǰ����������һ�δ���NULL���Ժ�ÿ��GetNext�ᱻ���£��������������
    // ����true��������һ����Ϣ������false�����������
    // ע��������û�м���
    bool GetNext(void*& msg, void*& iter);

private:
    pthread_mutex_t m_mutex;
    sem_t m_sem;

    MsgQueueNode m_queue;
    MemPool m_pool;
};

#endif

