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
    MemPool(size_t elemSize, size_t chunkSize);
    ~MemPool();

    void* Alloc();
    void Free(void* p);

private:
    void AddChunk();

private:
    size_t m_elemSize;
    size_t m_chunkSize;
    size_t m_chunkElems;

    MemPoolElem* m_freeElems;
    MemPoolChunk* m_chunks;
};

#endif

