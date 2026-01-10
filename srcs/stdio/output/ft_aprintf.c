/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_aprintf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 15:17:10 by syzygy            #+#    #+#             */
/*   Updated: 2025/11/21 15:17:54 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../format/format.h"
# include <stdlib.h>
# include <stdarg.h>

/*
 * Allocate and format a string using the existing ft_vsnprintf implementation.
 * Returns number of characters written (excluding NUL) on success and sets *dst
 * to a malloc'd buffer (caller must free). Returns -1 on error and sets *dst
 * to NULL.
 */
int	ft_aprintf(char **dst, const char *format, ...)
{
	va_list	ap;
	size_t	cap;
	char	*buf;
	int		ret;

	if (dst == NULL || format == NULL)
		return (-1);
	*dst = NULL;
	cap = 128;
	for (;;)
	{
		buf = (char *)malloc(cap);
		if (buf == NULL)
			return (-1);
		va_start(ap, format);
		ret = ft_vsnprintf(buf, cap, format, &ap);
		va_end(ap);
		if (ret < 0)
		{
			free(buf);
			return (ret);
		}
		if ((size_t)ret < cap - 1)
		{
			*dst = buf;
			return (ret);
		}
		free(buf);
		if (cap > (SIZE_MAX / 2))
			cap = SIZE_MAX;
		else
			cap *= 2;
		if (cap == SIZE_MAX && (size_t)ret >= cap - 1)
			return (-1);
	}
}
