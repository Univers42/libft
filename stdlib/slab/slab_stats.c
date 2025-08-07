/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   slab_stats.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 01:40:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/08 01:40:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "slab.h"
#include <stdio.h>

void		slab_stats(t_slab_allocator *slab)
{
	size_t			i;
	t_slab_cache	*cache;

	if (slab == NULL)
		return ;
	printf("Slab Allocator Stats:\n");
	i = 0;
	while (i < slab->cache_count)
	{
		cache = &slab->caches[i];
		printf("  Cache %zu (block_size=%zu):\n",
			i, cache->block_size);
		printf("    Allocated: %zu\n", cache->total_allocated);
		printf("    Freed: %zu\n", cache->total_freed);
		printf("    Active: %zu\n",
			cache->total_allocated - cache->total_freed);
		i += 1;
	}
}
