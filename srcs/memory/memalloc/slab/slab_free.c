/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   slab_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 01:40:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/07/28 00:00:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "slab.h"
#include <stdlib.h>
#include <stddef.h>

/* Exact owner lookup: scan each cache's chunks and accept the pointer only
   when slab_get_block_at_ptr proves it sits exactly on a block's data
   slot. Foreign heap pointers that merely fall inside the global lo/hi
   span (chunks are separate mallocs — the span has gaps) always miss. */
static t_slab_block	*slab_owning_block(t_slab_allocator *slab, void *ptr)
{
	size_t			i;
	t_slab_chunk	*chunk;
	t_slab_block	*blk;

	i = 0;
	while (i < slab->cache_count)
	{
		chunk = slab->caches[i].chunks;
		while (chunk != NULL)
		{
			blk = slab_get_block_at_ptr(chunk, &slab->caches[i], ptr);
			if (blk != NULL)
				return (blk);
			chunk = chunk->next;
		}
		i++;
	}
	return (NULL);
}

/* Free with EXACT ownership: a pointer outside the chunk span is libc
   memory (cheap reject, no deref); inside the span it is slab-owned only
   if some chunk really contains it on a block boundary. The old
   header-magic guess could misclassify a foreign pointer that landed in
   a span gap with SLAB_MAGIC bytes before it — silently dropping the
   free (a leak), or worse, pushing a fake block through a garbage chunk
   pointer. The magic check now only guards a corrupted genuine block. */
void	slab_free(t_slab_allocator *slab, void *ptr)
{
	t_slab_block	*block;
	t_slab_chunk	*chunk;

	if (slab == NULL || ptr == NULL)
		return ;
	if ((char *)ptr < slab->lo || (char *)ptr >= slab->hi)
		return ((void)fn_free(ptr));
	block = slab_owning_block(slab, ptr);
	if (block == NULL)
		return ((void)fn_free(ptr));
	if (block->magic != SLAB_MAGIC || block->is_free)
		return ;
	chunk = block->chunk;
	block->is_free = true;
	block->next = chunk->free_list;
	chunk->free_list = block;
	chunk->used_blocks -= 1;
}
