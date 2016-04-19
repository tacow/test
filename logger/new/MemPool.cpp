#include "MemPool.h"

MemPool::MemPool(size_t elemSize, size_t chunkSize) {
    if (elemSize < sizeof(MemPoolElem))
        elemSize = sizeof(MemPoolElem);

    m_elemSize = elemSize;
    m_chunkElems = chunkSize / elemSize;
    m_chunkSize = elemSize * m_chunkElems;
    m_freeElems = NULL;
    m_chunks = NULL;

    AddChunk();
}

MemPool::~MemPool() {
    MemPoolChunk* chunk = m_chunks;
    while(chunk) {
        MemPoolChunk* nextChunk = chunk->next;
        delete[] chunk->data;
        delete chunk;
        chunk = nextChunk;
    }
}

void* MemPool::Alloc() {
    if (!m_freeElems)
        AddChunk();

    MemPoolElem* elem = m_freeElems;
    m_freeElems = elem->next;

    return elem;
}

void MemPool::Free(void* p) {
    MemPoolElem* elem = (MemPoolElem*)p;
    elem->next = m_freeElems;
    m_freeElems = elem;
}

void MemPool::AddChunk() {
    MemPoolChunk* chunk = new MemPoolChunk;
    chunk->data = new char[m_chunkSize];
    chunk->next = m_chunks;
    m_chunks = chunk;

    for(size_t i = 0; i < m_chunkElems - 1; ++i) {
        MemPoolElem* elem = (MemPoolElem*)(chunk->data + i * m_elemSize);
        MemPoolElem* nextElem = (MemPoolElem*)(chunk->data + (i + 1) * m_elemSize);
        elem->next = nextElem;
    }
    MemPoolElem* lastElem = (MemPoolElem*)(chunk->data + (m_chunkElems - 1) * m_elemSize);
    lastElem->next = NULL;

    m_freeElems = (MemPoolElem*)chunk->data;
}

