/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   slab.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 01:40:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/08 01:40:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "slab.h"
#include <string.h>

void		slab_init(t_slab_allocator *slab)
{
	if (slab == NULL)
		return ;
	slab->cache_count = 0;
	memset(slab->caches, 0, sizeof(slab->caches));
}

void		slab_add_cache(t_slab_allocator *slab,
				size_t block_size, size_t blocks_per_chunk)
{
	t_slab_cache	*cache;

	if (slab == NULL)
		return ;
	if (slab->cache_count >= MAX_SLAB_SIZES)
		return ;
	cache = &slab->caches[slab->cache_count];
	cache->block_size = block_size;
	cache->blocks_per_chunk = blocks_per_chunk;
	cache->total_allocated = 0;
	cache->total_freed = 0;
	cache->chunks = NULL;
	slab->cache_count += 1;
}
	chunk->used_blocks = 0;
	chunk->total_blocks = cache->blocks_per_chunk;
	chunk->free_list = NULL;
	chunk->next = cache->chunks;
	ptr = chunk->memory;
	i = -1;
	while (++i < cache->blocks_per_chunk)
	{
		t_slab_block *block = (t_slab_block *)ptr;
		block->is_free = true;
		block->next = chunk->free_list;
		chunk->free_list = block;
		ptr += sizeof(t_slab_block) + cache->block_size;
	}
	return (chunk);
}

static t_slab_cache *find_best_cache(t_slab_allocator *slab, size_t size)
{
	size_t	i;
	t_slab_cache	*best;

	best = NULL;
	i = -1;
	while (++i < slab->cache_count)
	{
		if (slab->caches[i].block_size >= size)
			if (!best || slab->caches[i].block_size < best->block_size)
				best = &slab->caches[i];
	}
	return (best);
}

void *slab_alloc(t_slab_allocator *slab, size_t size)
{
	t_slab_cache *cache;
	t_slab_chunk	*chunk;

	if (!slab)
		return (NULL);	
	cache = find_best_cache(slab, size);
	if (!cache)
		return (malloc(size));
	chunk = cache->chunks;
	while (chunk && !chunk->free_list)
		chunk = chunk->next;
	if (!chunk)
	{
		chunk = create_slab_chunk(cache);
		if (!chunk)
			return (NULL);
		cache->chunks = chunk;
	}
	t_slab_block *block = chunk->free_list;
	chunk->free_list = block->next;
	block->is_free = false;
	chunk->used_blocks++;
	cache->total_allocated++;
	return (block->data);
}

void slab_free(t_slab_allocator *slab, void *ptr)
{
	size_t	i;
	t_slab_cache	*cache;
	t_slab_chunk	*chunk;
	char			*chunk_start;
	size_t			block_size;
	char			*chunk_end;
	size_t			offset;
	size_t			block_index;

	i = -1;
	if (!slab || !ptr)
		return ;
	while (++i < slab->cache_count)
	{
		cache = &slab->caches[i];
		chunk = cache->chunks;
		
		while (chunk)
		{
			chunk_start = chunk->memory;
			block_size = sizeof(t_slab_block) + cache->block_size;
			chunk_end = chunk_start + chunk->total_blocks * block_size;
			if ((char *)ptr >= chunk_start && (char *)ptr < chunk_end)
			{
				offset = (char *)ptr - chunk_start;
				block_index = offset / block_size;
				t_slab_block *block = (t_slab_block *)(chunk_start + block_index * block_size);
				if (block->data == ptr && !block->is_free)
				{
					block->is_free = true;
					block->next = chunk->free_list;
					chunk->free_list = block;
					chunk->used_blocks--;
					cache->total_freed++;
					return ;
				}
			}
			chunk = chunk->next;
		}
	}
	if (ptr > (void *)0x1000)
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
