/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsearch.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 00:55:39 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/30 01:11:46 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_memory.h"
#include "ft_string.h"
#include "ft_sort.h"
#include <stddef.h>

static inline void	bsearch_ctx_init(t_bsearch_ctx *ctx,
						const void *base, size_t nmemb, size_t size);

void	*bsearch_ctx(const void *key, const t_bsearch_ctx *ctx,
			int (*cmp)(const void *, const void *))
{
	const unsigned char	*b = (const unsigned char *)ctx->base;
	size_t				mididx;
	size_t				nmemb;
	const unsigned char	*midobj;
	int					diff;

	nmemb = ctx->nmemb;
	while (ctx->nmemb)
	{
		mididx = nmemb >> 1;
		midobj = b + mididx * ctx->size;
		diff = cmp(key, midobj);
		if (diff == 0)
			return ((void *) midobj);
		if (diff > 0)
		{
			b = midobj + ctx->size;
			nmemb -= mididx + 1;
		}
		else
			nmemb = mididx;
	}
	return (NULL);
}

static inline void	bsearch_ctx_init(t_bsearch_ctx *ctx,
				const void *base, size_t nmemb, size_t size)
{
	ctx->base = base;
	ctx->nmemb = nmemb;
	ctx->size = size;
}
