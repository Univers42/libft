/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strsub.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:00:51 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/30 17:39:05 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include "ft_string.h"

static char *ensure_capacity(t_subctx *c, size_t need)
{
	size_t n;

	n = c->tempsize ? c->tempsize * 2 : 16;
	while (n < need)
		n *= 2;
	c->temp = ft_realloc(c->temp, c->tempsize, n);
	if (!c->temp)
		return (NULL);
	c->tempsize = n;
	return (c->temp);
}

/* append replacement string 'rep' into ctx->temp */
static int append_rep(t_subctx *ctx, const char *rep)
{
	ctx->repn = ft_strlen(rep);
	if (!ensure_capacity(ctx, ctx->templen + ctx->repn + 1))
		return (-1);
	if (ctx->repn)
	{
		ft_memcpy(ctx->temp + ctx->templen, rep, ctx->repn);
		ctx->templen += ctx->repn;
	}
	return (0);
}

/* append remaining part of original string starting at idx */
static int append_rest(t_subctx *ctx, const char *string, size_t idx)
{
	ctx->remn = ft_strlen(string + idx);
	if (!ensure_capacity(ctx, ctx->templen + ctx->remn + 1))
		return (-1);
	if (ctx->remn)
	{
		ft_memcpy(ctx->temp + ctx->templen, string + idx, ctx->remn);
		ctx->templen += ctx->remn;
	}
	return (0);
}

/* append single character ch to buffer */
static int append_char(t_subctx *ctx, char ch)
{
	if (!ensure_capacity(ctx, ctx->templen + 2))
		return (-1);
	ctx->temp[ctx->templen++] = ch;
	return (0);
}

char *strsub(const char *string, const char *pat,
			 const char *rep, bool glob)
{
	t_subctx ctx = {0, 0, 0, 0, NULL, 0, 0};

	ctx.patlen = ft_strlen(pat);
	while (string[ctx.i])
	{
		if (ctx.patlen && ft_strncmp(string + ctx.i, pat, ctx.patlen) == 0)
		{
			if (append_rep(&ctx, rep) < 0)
				return (NULL);
			ctx.i += ctx.patlen ? ctx.patlen : 1;
			if (!glob)
			{
				if (append_rest(&ctx, string, ctx.i) < 0)
					return (NULL);
				break;
			}
		}
		else
			if (append_char(&ctx, string[ctx.i++]) < 0)
				return (NULL);
	}
	if (ctx.temp)
	{
		ctx.temp[ctx.templen] = '\0';
		return (ctx.temp);
	}
	return (ft_strdup(string));
}