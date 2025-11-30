/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   outmem.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 02:40:15 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/30 03:13:06 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "format.h"

/**
 * to put into memory.h
 */
void    outmem(const char *p, size_t len, t_out *dest)
{
	size_t  bufsize;
	size_t  offset;
	size_t  nleft;

	nleft = dest->end - dest->nextc;
	if (likely(nleft >= len))
	{
		ft_memcpy(dest, p, len);
		dest->nextc += len;
		return ;
	}
	bufsize = dest->buf_size;
	if(!bufsize)
		;
	else if (dest->buf == NULL)
	{
		offset = 0;
		if (bufsize < len)
			bufsize = len;
		bufsize = dest->buf_size;
		intoff();
		dest->buf = xrealloc(dest->buf, dest->buf_size, bufsize);
		dest->buf_size = bufsize;
		dest->end = dest->buf + bufsize;
		dest->nextc = dest->buf + offset;
		inton();
	}
	else
		flushout(dest);
	nleft = dest->end - dest->nextc;
	if (nleft >= len)
	{
		ft_memcpy(dest, p, len);
		dest->nextc += len;
		return ;
	}
	if ((ft_write(dest->fd, p, len)))
		dest->flags |= OUTPUT_ERR;
}