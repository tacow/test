#include <stdlib.h>
#include "mq.h"

#define CHUNK_SIZE (1024 * 1024)
#define CHUNK_NODES (CHUNK_SIZE / sizeof(mq_node_t))

void mq_addchunk(mq_t* mq) {
    chunk_t* chunk;
    int i;

    chunk = malloc(sizeof(chunk_t));
    chunk->nodes = malloc(CHUNK_SIZE);
    for(i = 0; i < CHUNK_NODES - 1; ++i)
        chunk->nodes[i].next = &chunk->nodes[i + 1];
    mq->free_nodes = &chunk->nodes[0];
    chunk->nodes[CHUNK_NODES - 1].next = NULL;

    chunk->next = mq->chunks;
    mq->chunks = chunk;
}

void mq_init(mq_t* mq) {
    pthread_mutex_init(&mq->mutex, NULL);
    pthread_cond_init(&mq->cond, NULL);
    mq->queue.next = &mq->queue;
    mq->queue.prev = &mq->queue;
    mq->free_nodes = NULL;
    mq->chunks = NULL;

    mq_addchunk(mq);
}

void mq_cleanup(mq_t* mq) {
    chunk_t* chunk;

    pthread_mutex_destroy(&mq->mutex);
    pthread_cond_destroy(&mq->cond);

    chunk = mq->chunks;
    while(chunk) {
        chunk_t* next_chunk;

        next_chunk = chunk->next;
        free(chunk);
        chunk = next_chunk;
    }
}

void mq_push(mq_t* mq, void* msg) {
    mq_node_t* node;
    mq_node_t* tail;
    int queue_empty;

    pthread_mutex_lock(&mq->mutex);

    if (!mq->free_nodes)
        mq_addchunk(mq);
    node = mq->free_nodes;
    mq->free_nodes = node->next;

    tail = mq->queue.prev;
    queue_empty = (tail == &mq->queue);
    node->msg = msg;
    node->next = tail->next;
    node->prev = tail;
    tail->next = node;
    mq->queue.prev = node;

    if (queue_empty)
        pthread_cond_signal(&mq->cond);

    pthread_mutex_unlock(&mq->mutex);
}

void* mq_pop(mq_t* mq) {
    mq_node_t* head;
    void* msg;

    pthread_mutex_lock(&mq->mutex);

    if (mq->queue.next == &mq->queue)
        pthread_cond_wait(&mq->cond, &mq->mutex);

    head = mq->queue.next;
    msg = head->msg;
    mq->queue.next = head->next;
    head->next->prev = &mq->queue;

    head->next = mq->free_nodes->next;
    mq->free_nodes->next = head;

    pthread_mutex_unlock(&mq->mutex);
    return msg;
}

