#include "MsgQueue.h"

#define MQ_CHUNK_SIZE (1024 * 1024)

MsgQueue::MsgQueue() : m_pool(sizeof(MsgQueueNode), MQ_CHUNK_SIZE) {
    pthread_mutex_init(&m_mutex, NULL);
    sem_init(&m_sem, 0, 0);

    m_queue.next = &m_queue;
    m_queue.prev = &m_queue;
}

MsgQueue::~MsgQueue() {
    pthread_mutex_destroy(&m_mutex);
    sem_destroy(&m_sem);
}

void MsgQueue::Push(void* msg) {
    pthread_mutex_lock(&m_mutex);

    MsgQueueNode* node = (MsgQueueNode*)m_pool.Alloc();

    MsgQueueNode* tail = m_queue.prev;
    node->msg = msg;
    node->next = tail->next;
    node->prev = tail;
    tail->next = node;
    m_queue.prev = node;

    pthread_mutex_unlock(&m_mutex);

    sem_post(&m_sem);
}

void* MsgQueue::Pop() {
    while(sem_wait(&m_sem) != 0) {
    }

    pthread_mutex_lock(&m_mutex);

    MsgQueueNode* head = m_queue.next;
    void* msg = head->msg;
    m_queue.next = head->next;
    head->next->prev = &m_queue;

    m_pool.Free(head);

    pthread_mutex_unlock(&m_mutex);
    return msg;
}

bool MsgQueue::GetNext(void*& msg, void*& iter) {
    MsgQueueNode* node;
    if (iter == NULL)
        node = &m_queue;
    else
        node = (MsgQueueNode*)iter;

    if (node->next == &m_queue)
        return false;

    node = node->next;
    msg = node->msg;
    iter = node;
    return true;
}

