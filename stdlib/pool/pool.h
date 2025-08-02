#ifndef POOL_H
#define POOL_H

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct s_pool_block
{
    struct s_pool_block *next;
    bool is_free;
    char data[];
} t_pool_block;

typedef struct s_pool_chunk
{
    struct s_pool_chunk *next;
    char *memory;
    size_t used_blocks;
    t_pool_block *free_list;
} t_pool_chunk;

typedef struct s_pool
{
    size_t block_size;
    size_t blocks_per_chunk;
    size_t total_allocated;
    size_t total_freed;
    t_pool_chunk *chunks;
} t_pool;

void pool_init(t_pool *pool, size_t block_size, size_t blocks_per_chunk);
void *pool_alloc(t_pool *pool);
void pool_free(t_pool *pool, void *ptr);
void pool_destroy(t_pool *pool);
void pool_stats(t_pool *pool);

#endif
