/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   slab_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 01:40:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/01/23 20:16:57 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "slab.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define START 0
#define PTR 1

static int	slab_free_in_cache(t_slab_cache *cache, void *ptr)
{
	t_slab_chunk	*chunk;
	t_slab_block	*blk;

	chunk = cache->chunks;
	while (chunk != NULL)
	{
		blk = slab_get_block_at_ptr(chunk, cache, ptr);
		if (blk != NULL && blk->is_free == false)
		{
			blk->is_free = true;
			blk->next = chunk->free_list;
			chunk->free_list = blk;
			chunk->used_blocks -= 1;
			cache->total_freed += 1;
			return (1);
		}
		if (blk != NULL)
			return (0);
		chunk = chunk->next;
	}
	return (0);
}

void	slab_free(t_slab_allocator *slab, void *ptr)
{
	size_t	i;

	if (slab == NULL || ptr == NULL)
		return ;
	i = 0;
	while (i < slab->cache_count)
	{
		if (slab_free_in_cache(&slab->caches[i], ptr))
			return ;
		i += 1;
	}
	if (ptr > (void *)0x1000)
		free(ptr);
}
