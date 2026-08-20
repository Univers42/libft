/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   writer2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/10 23:13:56 by marvin            #+#    #+#             */
/*   Updated: 2026/01/10 23:13:56 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "format.h"

/* Write `n` in base ten, zero-padded to `digits`.

   The signed and hex writers both used to route their recursion through
   ft_abs(), which is declared `int ft_abs(int)`. Every value wider than 32
   bits was therefore truncated on the way down: %llx of 0xdeadbeefcafe
   printed 24110351e, and %lld of LLONG_MIN+1 printed nonsense. The value
   here is already unsigned, so there is nothing to take an absolute value
   OF -- the call was both lossy and pointless. */
void	writer_unsigned(t_buffer *buff, size_t n, int digits)
{
	if (n / 10 || digits > 0)
		writer_unsigned(buff, n / 10, digits - 1);
	if (digits > 0)
		writer_char(buff, (char)(n % 10) + '0');
}

void	writer_hex_lowcase(t_buffer *buff, size_t n, int add_prefix, int digits)
{
	if (n != 0 && add_prefix)
	{
		writer_char(buff, '0');
		writer_char(buff, 'x');
	}
	if (n / 16 || digits > 1)
		writer_hex_lowcase(buff, n / 16, 0, digits - 1);
	if (digits > 0)
		writer_char(buff, "0123456789abcdef"[n % 16]);
}

void	writer_hex_upcase(t_buffer *buff, size_t n, int add_prefix, int digits)
{
	if (n != 0 && add_prefix)
	{
		writer_char(buff, '0');
		writer_char(buff, 'X');
	}
	if (n / 16 || digits > 1)
		writer_hex_upcase(buff, n / 16, 0, digits - 1);
	if (digits > 0)
		writer_char(buff, "0123456789ABCDEF"[n % 16]);
}

int	writer_init(t_buffer *ret)
{
	*ret = (t_buffer){0};
	ret->cap = 50;
	ret->buff = fn_malloc(ret->cap + 1);
	if (!ret->buff)
		return (1);
	ret->buff[0] = 0;
	ret->out_fd = 1;
	return (0);
}
