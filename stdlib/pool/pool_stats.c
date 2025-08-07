/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pool_stats.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 01:55:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/08 01:55:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pool.h"
#include <stdio.h>

void	pool_stats(t_pool *pool)
{
	if (pool == NULL)
		return ;
	printf("Pool Stats:\n");
	printf("  Block size: %zu bytes\n", pool->block_size);
	printf("  Total allocated: %zu\n", pool->total_allocated);
	printf("  Total freed: %zu\n", pool->total_freed);
	printf("  Active allocations: %zu\n",
		pool->total_allocated - pool->total_freed);
}
