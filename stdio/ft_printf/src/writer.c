/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   writer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 00:47:50 by dlesieur          #+#    #+#             */
/*   Updated: 2025/09/08 13:43:58 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "writer.h"
#include "ft_memory.h" /* for ft_memcpy */

/*
 * Does not zero initialize the buffer. Zero initializes the variables.
 */
void	writer_reset(t_writer *w)
{
	w->index = 0;
	w->error = 0;
	w->n_written = 0;
}

bool	writer_write(t_writer *w, const char *mem, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size && writer_putchar(w, mem[i]))
		++i;
	return (w->error >= 0);
}

int	writer_flush(t_writer *w)
{
	size_t	n_written;
	size_t	total_written;

	if (w->mode == WRITER_MODE_BUF)
	{
		size_t	can_copy = w->index;
		size_t	space = (w->n_written < (int)w->dst_cap)
			? (w->dst_cap - (size_t)w->n_written) : 0;
		if (can_copy > space)
		{
			/* truncate to available space, mark error */
			can_copy = space;
			w->error = -1;
		}
		if (can_copy > 0 && w->dst != NULL)
			ft_memcpy(w->dst + w->n_written, w->buf, can_copy);
		w->n_written += (int)can_copy;
		w->index = 0;
		return ((int)can_copy);
	}

	/* default: FD mode */
	total_written = 0;
	while (total_written < w->index)
	{
		n_written = write(w->fd, w->buf + total_written,
				w->index - total_written);
		if ((ssize_t)n_written < 0)
			return (w->error = (int)n_written, (int)n_written);
		total_written += n_written;
	}
	w->index = 0;
	w->n_written += (int)total_written;
	return ((int)total_written);
}

bool	writer_is_full(t_writer *w)
{
	return (w->index == BUF_SIZE);
}

bool	writer_putchar(t_writer *w, char c)
{
	if (writer_is_full(w) && writer_flush(w) < 0)
		return (false);
	w->buf[w->index] = c;
	w->index++;
	return (true);
}

int	writer_terminate_cstr(t_writer *w)
{
	if (w->mode != WRITER_MODE_BUF)
		return (0);
	/* flush any pending buffered bytes first */
	if (w->index && writer_flush(w) < 0 && w->dst_cap == 0)
		return (-1);
	/* write trailing '\0' if there's room for it */
	if (w->dst && (size_t)w->n_written < w->dst_cap + 1)
	{
		/* we reserved dst_cap for data; NUL goes after it if space exists */
		if ((size_t)w->n_written < w->dst_cap)
			w->dst[w->n_written] = '\0';
		else
			w->dst[w->dst_cap] = '\0';
		return (0);
	}
	w->error = -1;
	return (-1);
}