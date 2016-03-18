#ifndef MEM_POOL_H
#define MEM_POOL_H

typedef struct mem_pool_elem_s mem_pool_elem_t;

struct mem_pool_elem_s {
    mem_pool_elem_t* next;
};

typedef struct mem_pool_chunk_s mem_pool_chunk_t;

struct mem_pool_chunk_s {
    mem_pool_chunk_t* next;
    char* data;
};

typedef struct mem_pool_s mem_pool_t;

struct mem_pool_s {
    int elem_size;
    int chunk_size;
    int chunk_elems;

    mem_pool_elem_t* free_elems;
    mem_pool_chunk_t* chunks;
};

void mem_pool_init(mem_pool_t* mem_pool, int elem_size, int chunk_size);
void mem_pool_cleanup(mem_pool_t* mem_pool);

void* mem_pool_alloc(mem_pool_t* mem_pool);
void mem_pool_free(mem_pool_t* mem_pool, void* p);

#endif

