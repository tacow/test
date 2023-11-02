#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BUFFER_SIZE 5           // 缓冲区大小
#define NUM_PRODUCERS 2         // 生产者数量
#define NUM_CONSUMERS 2         // 消费者数量
#define NUM_ITEMS_TO_PRODUCE 10 // 生产物品的总数量

int buffer[BUFFER_SIZE];
int buffer_index = 0;

pthread_mutex_t buffer_mutex;
pthread_cond_t buffer_full_cond, buffer_empty_cond;

// 生产者线程函数
void *producer(void *arg)
{
    int *producer_id = (int *)arg;
    int i;

    for (i = 0; i < NUM_ITEMS_TO_PRODUCE; i++)
    {
        pthread_mutex_lock(&buffer_mutex);

        // 如果缓冲区已满，等待消费者消费
        while (buffer_index == BUFFER_SIZE)
        {
            pthread_cond_wait(&buffer_full_cond, &buffer_mutex);
        }

        // 生产物品并添加到缓冲区
        buffer[buffer_index] = i;
        buffer_index++;

        printf("Producer %d produced item: %d\n", *producer_id, i);

        // 通知消费者可以消费了
        pthread_cond_signal(&buffer_empty_cond);

        pthread_mutex_unlock(&buffer_mutex);
    }

    pthread_exit(NULL);
}

// 消费者线程函数
void *consumer(void *arg)
{
    int *consumer_id = (int *)arg;
    int i;

    for (i = 0; i < NUM_ITEMS_TO_PRODUCE; i++)
    {
        pthread_mutex_lock(&buffer_mutex);

        // 如果缓冲区为空，等待生产者生产
        while (buffer_index == 0)
        {
            pthread_cond_wait(&buffer_empty_cond, &buffer_mutex);
        }

        // 从缓冲区获取物品并消费
        int item = buffer[buffer_index - 1];
        buffer_index--;

        printf("Consumer %d consumed item: %d\n", *consumer_id, item);

        // 通知生产者可以继续生产了
        pthread_cond_signal(&buffer_full_cond);

        pthread_mutex_unlock(&buffer_mutex);
    }

    pthread_exit(NULL);
}

int main()
{
    pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];
    int i;

    // 初始化互斥锁和条件变量
    pthread_mutex_init(&buffer_mutex, NULL);
    pthread_cond_init(&buffer_full_cond, NULL);
    pthread_cond_init(&buffer_empty_cond, NULL);

    // 创建生产者线程
    for (i = 0; i < NUM_PRODUCERS; i++)
    {
        int *producer_id = (int *)malloc(sizeof(int));
        *producer_id = i;
        pthread_create(&producers[i], NULL, producer, producer_id);
    }

    // 创建消费者线程
    for (i = 0; i < NUM_CONSUMERS; i++)
    {
        int *consumer_id = (int *)malloc(sizeof(int));
        *consumer_id = i;
        pthread_create(&consumers[i], NULL, consumer, consumer_id);
    }

    // 等待生产者线程完成
    for (i = 0; i < NUM_PRODUCERS; i++)
    {
        pthread_join(producers[i], NULL);
    }

    // 等待消费者线程完成
    for (i = 0; i < NUM_CONSUMERS; i++)
    {
        pthread_join(consumers[i], NULL);
    }

    // 销毁互斥锁和条件变量
    pthread_mutex_destroy(&buffer_mutex);
    pthread_cond_destroy(&buffer_full_cond);
    pthread_cond_destroy(&buffer_empty_cond);

    return 0;
}