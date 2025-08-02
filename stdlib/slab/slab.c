#include "slab.h"
#include <stdio.h>
#include <string.h>

void slab_init(t_slab_allocator *slab)
{
    slab->cache_count = 0;
    memset(slab->caches, 0, sizeof(slab->caches));
}

void slab_add_cache(t_slab_allocator *slab, size_t block_size, size_t blocks_per_chunk)
{
    if (slab->cache_count >= MAX_SLAB_SIZES)
        return;
    
    t_slab_cache *cache = &slab->caches[slab->cache_count++];
    cache->block_size = block_size;
    cache->blocks_per_chunk = blocks_per_chunk;
    cache->total_allocated = 0;
    cache->total_freed = 0;
    cache->chunks = NULL;
}

static t_slab_chunk *create_slab_chunk(t_slab_cache *cache)
{
    size_t chunk_size = sizeof(t_slab_chunk);
    size_t memory_size = cache->blocks_per_chunk * (sizeof(t_slab_block) + cache->block_size);
    
    t_slab_chunk *chunk = malloc(chunk_size);
    if (!chunk)
        return NULL;
    
    chunk->memory = malloc(memory_size);
    if (!chunk->memory)
    {
        free(chunk);
        return NULL;
    }
    
    chunk->block_size = cache->block_size;
    chunk->used_blocks = 0;
    chunk->total_blocks = cache->blocks_per_chunk;
    chunk->free_list = NULL;
    chunk->next = cache->chunks;
    
    // Initialize free list
    char *ptr = chunk->memory;
    for (size_t i = 0; i < cache->blocks_per_chunk; i++)
    {
        t_slab_block *block = (t_slab_block *)ptr;
        block->is_free = true;
        block->next = chunk->free_list;
        chunk->free_list = block;
        ptr += sizeof(t_slab_block) + cache->block_size;
    }
    
    return chunk;
}

static t_slab_cache *find_best_cache(t_slab_allocator *slab, size_t size)
{
    t_slab_cache *best = NULL;
    
    for (size_t i = 0; i < slab->cache_count; i++)
    {
        if (slab->caches[i].block_size >= size)
        {
            if (!best || slab->caches[i].block_size < best->block_size)
                best = &slab->caches[i];
        }
    }
    
    return best;
}

void *slab_alloc(t_slab_allocator *slab, size_t size)
{
    if (!slab)
        return NULL;
    
    t_slab_cache *cache = find_best_cache(slab, size);
    if (!cache)
        return malloc(size); // Fallback to malloc for oversized allocations
    
    // Find a chunk with free blocks
    t_slab_chunk *chunk = cache->chunks;
    while (chunk && !chunk->free_list)
        chunk = chunk->next;
    
    // No free blocks, create new chunk
    if (!chunk)
    {
        chunk = create_slab_chunk(cache);
        if (!chunk)
            return NULL;
        cache->chunks = chunk;
    }
    
    // Get block from free list
    t_slab_block *block = chunk->free_list;
    chunk->free_list = block->next;
    block->is_free = false;
    chunk->used_blocks++;
    cache->total_allocated++;
    
    return block->data;
}

void slab_free(t_slab_allocator *slab, void *ptr)
{
    if (!slab || !ptr)
        return;
    
    // Find the chunk containing this pointer
    for (size_t i = 0; i < slab->cache_count; i++)
    {
        t_slab_cache *cache = &slab->caches[i];
        t_slab_chunk *chunk = cache->chunks;
        
        while (chunk)
        {
            char *chunk_start = chunk->memory;
            size_t block_size = sizeof(t_slab_block) + cache->block_size;
            char *chunk_end = chunk_start + chunk->total_blocks * block_size;
            
            if ((char *)ptr >= chunk_start && (char *)ptr < chunk_end)
            {
                // Calculate block address - fix the offset calculation
                size_t offset = (char *)ptr - chunk_start;
                size_t block_index = offset / block_size;
                
                // Get the actual block start
                t_slab_block *block = (t_slab_block *)(chunk_start + block_index * block_size);
                
                // Verify this is the correct block and it points to our data
                if (block->data == ptr && !block->is_free)
                {
                    block->is_free = true;
                    block->next = chunk->free_list;
                    chunk->free_list = block;
                    chunk->used_blocks--;
                    cache->total_freed++;
                    return;
                }
            }
            chunk = chunk->next;
        }
    }
    
    // Not found in any cache, assume it was malloc'd - but be careful
    // Only free if it's a reasonable pointer
    if (ptr > (void *)0x1000) // Basic sanity check
        free(ptr);
}

void slab_destroy(t_slab_allocator *slab)
{
    if (!slab)
        return;
    
    for (size_t i = 0; i < slab->cache_count; i++)
    {
        t_slab_cache *cache = &slab->caches[i];
        t_slab_chunk *chunk = cache->chunks;
        
        while (chunk)
        {
            t_slab_chunk *next = chunk->next;
            free(chunk->memory);
            free(chunk);
            chunk = next;
        }
        cache->chunks = NULL;
    }
}

void slab_stats(t_slab_allocator *slab)
{
    if (!slab)
        return;
    
    printf("Slab Allocator Stats:\n");
    for (size_t i = 0; i < slab->cache_count; i++)
    {
        t_slab_cache *cache = &slab->caches[i];
        printf("  Cache %zu (block_size=%zu):\n", i, cache->block_size);
        printf("    Allocated: %zu\n", cache->total_allocated);
        printf("    Freed: %zu\n", cache->total_freed);
        printf("    Active: %zu\n", cache->total_allocated - cache->total_freed);
    }
}
