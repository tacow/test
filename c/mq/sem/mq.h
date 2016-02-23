#ifndef MQ_H
#define MQ_H

#include <pthread.h>
#include <semaphore.h>

typedef struct mq_node_s mq_node_t;

struct mq_node_s {
    mq_node_t* next;
    mq_node_t* prev;
    void* msg;
};

typedef struct chunk_s chunk_t;

struct chunk_s {
    chunk_t* next;
    mq_node_t* nodes;
};

typedef struct mq_s mq_t;

struct mq_s {
    pthread_mutex_t mutex;
    sem_t sem;

    mq_node_t queue;
    mq_node_t* free_nodes;
    chunk_t* chunks;
};

void mq_init(mq_t* mq);
void mq_cleanup(mq_t* mq);

void mq_push(mq_t* mq, void* msg);
void* mq_pop(mq_t* mq);

#endif

