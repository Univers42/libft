#include "pool.h"
#include <stdio.h>
#include <string.h>

void pool_init(t_pool *pool, size_t block_size, size_t blocks_per_chunk)
{
    pool->block_size = block_size;
    pool->blocks_per_chunk = blocks_per_chunk;
    pool->total_allocated = 0;
    pool->total_freed = 0;
    pool->chunks = NULL;
}

static t_pool_chunk *create_chunk(t_pool *pool)
{
    size_t chunk_size = sizeof(t_pool_chunk);
    size_t memory_size = pool->blocks_per_chunk * (sizeof(t_pool_block) + pool->block_size);
    
    t_pool_chunk *chunk = malloc(chunk_size);
    if (!chunk)
        return NULL;
    
    chunk->memory = malloc(memory_size);
    if (!chunk->memory)
    {
        free(chunk);
        return NULL;
    }
    
    chunk->used_blocks = 0;
    chunk->free_list = NULL;
    chunk->next = pool->chunks;
    
    // Initialize free list
    char *ptr = chunk->memory;
    for (size_t i = 0; i < pool->blocks_per_chunk; i++)
    {
        t_pool_block *block = (t_pool_block *)ptr;
        block->is_free = true;
        block->next = chunk->free_list;
        chunk->free_list = block;
        ptr += sizeof(t_pool_block) + pool->block_size;
    }
    
    return chunk;
}

void *pool_alloc(t_pool *pool)
{
    if (!pool)
        return NULL;
    
    // Find a chunk with free blocks
    t_pool_chunk *chunk = pool->chunks;
    while (chunk && !chunk->free_list)
        chunk = chunk->next;
    
    // No free blocks, create new chunk
    if (!chunk)
    {
        chunk = create_chunk(pool);
        if (!chunk)
            return NULL;
        pool->chunks = chunk;
    }
    
    // Get block from free list
    t_pool_block *block = chunk->free_list;
    chunk->free_list = block->next;
    block->is_free = false;
    chunk->used_blocks++;
    pool->total_allocated++;
    
    return block->data;
}

void pool_free(t_pool *pool, void *ptr)
{
    if (!pool || !ptr)
        return;
    
    // Find the block containing this pointer
    t_pool_chunk *chunk = pool->chunks;
    while (chunk)
    {
        char *chunk_start = chunk->memory;
        char *chunk_end = chunk_start + pool->blocks_per_chunk * (sizeof(t_pool_block) + pool->block_size);
        
        if ((char *)ptr >= chunk_start && (char *)ptr < chunk_end)
        {
            // Calculate block address
            t_pool_block *block = (t_pool_block *)((char *)ptr - sizeof(t_pool_block));
            
            if (!block->is_free)
            {
                block->is_free = true;
                block->next = chunk->free_list;
                chunk->free_list = block;
                chunk->used_blocks--;
                pool->total_freed++;
            }
            return;
        }
        chunk = chunk->next;
    }
}

void pool_destroy(t_pool *pool)
{
    if (!pool)
        return;
    
    t_pool_chunk *chunk = pool->chunks;
    while (chunk)
    {
        t_pool_chunk *next = chunk->next;
        free(chunk->memory);
        free(chunk);
        chunk = next;
    }
    pool->chunks = NULL;
}

void pool_stats(t_pool *pool)
{
    if (!pool)
        return;
    
    printf("Pool Stats:\n");
    printf("  Block size: %zu bytes\n", pool->block_size);
    printf("  Total allocated: %zu\n", pool->total_allocated);
    printf("  Total freed: %zu\n", pool->total_freed);
    printf("  Active allocations: %zu\n", pool->total_allocated - pool->total_freed);
}
