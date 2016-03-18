#include <stdlib.h>
#include "mem_pool.h"

static void mem_pool_addchunk(mem_pool_t* mem_pool) {
    mem_pool_chunk_t* chunk;
    mem_pool_elem_t* last_elem;
    int i;

    chunk = malloc(sizeof(mem_pool_chunk_t));
    chunk->data = malloc(mem_pool->chunk_size);
    chunk->next = mem_pool->chunks;
    mem_pool->chunks = chunk;

    for(i = 0; i < mem_pool->chunk_elems - 1; ++i) {
        mem_pool_elem_t* elem;
        mem_pool_elem_t* next_elem;

        elem = (mem_pool_elem_t*)(chunk->data + i * mem_pool->elem_size);
        next_elem = (mem_pool_elem_t*)(chunk->data + (i + 1) * mem_pool->elem_size);
        elem->next = next_elem;
    }
    last_elem = (mem_pool_elem_t*)(chunk->data + (mem_pool->chunk_elems - 1) * mem_pool->elem_size);
    last_elem->next = NULL;

    mem_pool->free_elems = (mem_pool_elem_t*)chunk->data;
}

void mem_pool_init(mem_pool_t* mem_pool, int elem_size, int chunk_size) {
    if (elem_size < sizeof(mem_pool_elem_t))
        elem_size = sizeof(mem_pool_elem_t);

    mem_pool->elem_size = elem_size;
    mem_pool->chunk_elems = chunk_size / elem_size;
    mem_pool->chunk_size = elem_size * mem_pool->chunk_elems;
    mem_pool->free_elems = NULL;
    mem_pool->chunks = NULL;

    mem_pool_addchunk(mem_pool);
}

void mem_pool_cleanup(mem_pool_t* mem_pool) {
    mem_pool_chunk_t* chunk;
    mem_pool_chunk_t* next_chunk;

    chunk = mem_pool->chunks;
    while(chunk) {
        next_chunk = chunk->next;
        free(chunk->data);
        free(chunk);
        chunk = next_chunk;
    }
}

void* mem_pool_alloc(mem_pool_t* mem_pool) {
    mem_pool_elem_t* elem;

    if (!mem_pool->free_elems)
        mem_pool_addchunk(mem_pool);

    elem = mem_pool->free_elems;
    mem_pool->free_elems = elem->next;

    return elem;
}

void mem_pool_free(mem_pool_t* mem_pool, void* p) {
    mem_pool_elem_t* elem;

    elem = (mem_pool_elem_t*)p;
    elem->next = mem_pool->free_elems;
    mem_pool->free_elems = elem;
}

