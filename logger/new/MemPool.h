#ifndef MEM_POOL_H
#define MEM_POOL_H

#include <stddef.h>

struct MemPoolElem {
    MemPoolElem* next;
};

struct MemPoolChunk {
    MemPoolChunk* next;
    char* data;
};

class MemPool {
public:
    MemPool(size_t elemSize, size_t chunkSize, void (*ctor)(void*) = NULL, void (*dtor)(void*) = NULL);
    ~MemPool();

    void* Alloc();
    void Free(void* p);

private:
    void AddChunk();

private:
    size_t m_elemSize;
    size_t m_chunkSize;
    size_t m_chunkElems;

    void (*m_ctor)(void*);
    void (*m_dtor)(void*);

    MemPoolElem* m_freeElems;
    MemPoolChunk* m_chunks;
};

template<class T>
void DefCtor(void* p) {
    new(p) T();
}

template<class T>
void DefDtor(void* p) {
    ((T*)p)->~T();
}

#endif

