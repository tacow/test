#include "ref_count.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string>

using namespace std;

#define ARRAY_LEN           10000

#define SET_THREADS         10
#define SET_NULL_THREADS    3
#define SET_TIMES           100000

#define GET_THREADS         20
#define GET_TIMES           100000

class Data : public RefCountObject<Data> {
public:
    int i;
    string s;
};

typedef RefCountObjectPtr<Data> DataPtr;

DataPtr g_dataPtrs[ARRAY_LEN];

int Rand(int bound) {
    return rand() % bound;
}

Data* CreateData() {
    Data* data = new Data();
    data->i = rand();

    char buf[6];
    for(int i = 0; i < 5; ++i)
        buf[i] = 'a' + Rand(26);
    buf[5] = '\0';
    data->s = buf;

    return data;
}

void* SetThread(void* ptr) {
    int tid = (long)ptr;
    printf("Set thread %d start\n", tid);

    for(int i = 0; i < SET_TIMES; ++i) {
        Data* data = NULL;
        if (tid >= SET_NULL_THREADS)
            data = CreateData();

        int index = Rand(ARRAY_LEN);
        g_dataPtrs[index].Set(data);

        if (data)
            data->Release();
    }

    printf("Set thread %d end\n", tid);
    return NULL;
}

void* GetThread(void* ptr) {
    int tid = (long)ptr;
    printf("Get thread %d start\n", tid);

    char filename[256];
    snprintf(filename, 256, "t%d.txt", tid);
    FILE* f = fopen(filename, "w");
    for(int i = 0; i < GET_TIMES; ++i) {
        int index = Rand(ARRAY_LEN);
        Data* data = g_dataPtrs[index].Get();
        if (data) {
            fprintf(f, "%d %s\n", data->i, data->s.c_str());
            data->Release();
        } else {
            fprintf(f, "NULL\n");
        }
    }
    fclose(f);

    printf("Get thread %d end\n", tid);
    return NULL;
}

int main() {
    pthread_t setThreads[SET_THREADS];
    pthread_t getThreads[GET_THREADS];

    for(int i = 0; i < SET_THREADS; ++i)
        pthread_create(&setThreads[i], NULL, SetThread, (void*)i);
    for(int i = 0; i < GET_THREADS; ++i)
        pthread_create(&getThreads[i], NULL, GetThread, (void*)i);

    for(int i = 0; i < SET_THREADS; ++i)
        pthread_join(setThreads[i], NULL);
    for(int i = 0; i < GET_THREADS; ++i)
        pthread_join(getThreads[i], NULL);

    return 0;
}

