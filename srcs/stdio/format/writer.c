/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   writer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/10 23:13:53 by marvin            #+#    #+#             */
/*   Updated: 2026/01/10 23:13:53 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "format.h"

int	writer_buffer_double(t_buffer *ret)
{
	int			i;
	char		*buff;

	ret->cap = ret->cap * 2;
	buff = fn_malloc(ret->cap + 1);
	if (buff == 0)
		return (1);
	ret->buff[ret->len] = 0;
	i = -1;
	while (++i < ret->len)
		buff[i] = ret->buff[i];
	fn_free(ret->buff);
	ret->buff = buff;
	return (0);
}

void	writer_char(t_buffer *buffer, char c)
{
	ssize_t	res;

	buffer->total_put_in++;
	if (buffer->len == buffer->cap)
	{
		if (writer_buffer_double(buffer))
		{
			if (!buffer->no_write)
				ft_putmem(buffer->buff, buffer->len);
			fn_free(buffer->buff);
			buffer->buff = 0;
		}
	}
	if (buffer->buff)
	{
		buffer->buff[buffer->len++] = c;
		buffer->buff[buffer->len] = 0;
	}
	else if (!buffer->no_write)
	{
		res = write(buffer->out_fd, &c, 1);
		(void)res;
	}
}

void	writer_padn(t_buffer *buff, char padding, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		writer_char(buff, padding);
		n--;
	}
}

void	writer_strn(t_buffer *buff, char *str, int n)
{
	int	i;

	i = 0;
	while (i < n && str[i])
	{
		writer_char(buff, str[i]);
		i++;
	}
}

/* Emit the sign, then the magnitude.

   The magnitude is taken in size_t rather than by negating the ssize_t,
   because negating the most negative value is undefined -- and it is
   exactly the value someone printing %lld is most likely to be testing
   with. Unsigned arithmetic gives the right answer for it with no special
   case. */
void	writer_signed(t_buffer *buff, ssize_t n, int add_sign, int digits)
{
	size_t	mag;

	if (n < 0)
		writer_char(buff, '-');
	else if (add_sign)
		writer_char(buff, '+');
	mag = (size_t)n;
	if (n < 0)
		mag = -(size_t)n;
	writer_unsigned(buff, mag, digits);
}
