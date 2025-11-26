/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_alloc_buffer.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 16:11:59 by syzygy            #+#    #+#             */
/*   Updated: 2025/11/21 16:25:45 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "format.h"

int ft_asprintf(char **sp, const char *f, ...)
{
	va_list ap;
	int ret;

	va_start(ap, f);
	ret = xvasprintf(sp, 0, f, ap);
	va_end(ap);
	return ret;
}

/**
 * printf to allocated string
 */
int ft_vasprintf(char **strp, size_t size, const char *fmt, va_list ap)
{
	char    *s;
	int 	len;
	va_list	ap2;

	va_copy(ap2, ap);
	len = ft_vsnprintf(*strp, size, fmt, ap2);
	va_end(ap2);
	if (len < 0)
		return (len);
	if (len < size)
		return (len);
	if (len >= stack_block_size())
		s = st_alloc(len + 1);
	else
		s = st_alloc(stack_block_size() + 1);
	*strp = s;
	len = ft_vsnprintf(s, len + 1, fmt, ap);
	return (len);
}
