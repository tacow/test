#include "mq.h"

#define MQ_CHUNK_SIZE (1024 * 1024)

void mq_init(mq_t* mq) {
    pthread_mutex_init(&mq->mutex, NULL);
    sem_init(&mq->sem, 0, 0);
    mq->queue.next = &mq->queue;
    mq->queue.prev = &mq->queue;
    mem_pool_init(&mq->mem_pool, sizeof(mq_node_t), MQ_CHUNK_SIZE);
}

void mq_cleanup(mq_t* mq) {
    pthread_mutex_destroy(&mq->mutex);
    sem_destroy(&mq->sem);
    mem_pool_cleanup(&mq->mem_pool);
}

void mq_push(mq_t* mq, void* msg) {
    mq_node_t* node;
    mq_node_t* tail;

    pthread_mutex_lock(&mq->mutex);

    node = mem_pool_alloc(&mq->mem_pool);

    tail = mq->queue.prev;
    node->msg = msg;
    node->next = tail->next;
    node->prev = tail;
    tail->next = node;
    mq->queue.prev = node;

    pthread_mutex_unlock(&mq->mutex);

    sem_post(&mq->sem);
}

void* mq_pop(mq_t* mq) {
    mq_node_t* head;
    void* msg;

    while(sem_wait(&mq->sem) != 0) {
    }

    pthread_mutex_lock(&mq->mutex);

    head = mq->queue.next;
    msg = head->msg;
    mq->queue.next = head->next;
    head->next->prev = &mq->queue;

    mem_pool_free(&mq->mem_pool, head);

    pthread_mutex_unlock(&mq->mutex);
    return msg;
}

