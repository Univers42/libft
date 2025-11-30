/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   outmem.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 16:30:28 by syzygy            #+#    #+#             */
/*   Updated: 2025/11/30 02:36:20 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "output.h"
#include "ft_string.h"
#include "ft_memory.h"

void outmem(const char *p, size_t len, t_out *dst)
{
	size_t buf_size;
	size_t offset;
	size_t n_left;

	n_left = dst->end - dst->nextc;
	if (n_left >= len)
	{
		ft_memcpy(dst->nextc, p, len);
		dst->nextc += len;
		return;
	}
	buf_size = dst->buf_size;
	if (buf_size != 0 && dst->buf == NULL)
	{
		offset = 0;
		dst->buf = xrealloc(dst->buf, 0, buf_size);
		dst->buf_size = buf_size;
		dst->end = dst->buf + buf_size;
		dst->nextc = dst->buf + offset;
	}
	else if (dst->buf != NULL)
		flushout(dst);
	n_left = dst->end - dst->nextc;
	if (n_left >= len)
	{
		ft_memcpy(dst->nextc, p, len);
		dst->nextc += len;
		return;
	}
	if (ft_write(dst->fd, p, len))
		dst->flags |= OUTPUT_ERR;
}