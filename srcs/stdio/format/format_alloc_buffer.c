/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_alloc_buffer.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 16:11:59 by syzygy            #+#    #+#             */
/*   Updated: 2025/11/30 03:30:14 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "format.h"

int ft_asprintf(char **sp, const char *f, ...)
{
	va_list ap;
	int ret;

	va_start(ap, f);
	ret = ft_vasprintf(sp, 0, f, ap);
	va_end(ap);
	return ret;
}

int xasprintf(char **sp, const char *f, ...)
{
	va_list ap;
	int ret;

	va_start(ap, f);
	ret = ft_vasprintf(sp, 0, f, ap);
	va_end(ap);
	return ret;
}

/**
 *
 * printf to allocated string
 */
int ft_vasprintf(char **strp, size_t size, const char *fmt, va_list ap)
{
	char *s;
	int len;
	va_list ap2;

	va_copy(ap2, ap);
	len = xvsnprintf(*strp, size, fmt, &ap2);
	va_end(ap2);
	if (len < 0)
		sh_error("xvsnprintf failed");
	/* compare using same unsignedness to avoid signed/unsigned warnings */
	if (size != 0 && (size_t)len < size)
		return (len);
	if ((size_t)len >= stack_block_size())
		s = st_alloc((size_t)len + 1);
	else
		s = st_alloc(stack_block_size() + 1);
	*strp = s;
	/* prepare fresh copy of ap for the second formatting call */
	va_copy(ap2, ap);
	len = ft_vsnprintf(s, (size_t)len + 1, fmt, &ap2);
	va_end(ap2);
	return (len);
}
