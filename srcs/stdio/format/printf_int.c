/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_int.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/10 23:13:27 by marvin            #+#    #+#             */
/*   Updated: 2026/01/10 23:13:27 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "format.h"

void	printf_int(va_list *args, t_fmt_spec spec, t_buffer *buff)
{
	ssize_t	i;
	int		pad_len;
	int		total_len;
	int		digits;

	if (spec.t != FMT_INTEGER)
		return ;
	i = fetch_signed(args, spec.length);
	spec = normilize_int_spec(spec, i);
	total_len = signed_total_len(i, spec);
	pad_len = signed_padding_len(i, spec);
	digits = signed_num_digits(i);
	if (!(spec.flags & FL_MINUS) && spec.width != -1)
		writer_padn(buff, ' ', spec.width - total_len);
	if (i >= 0 && !(spec.flags & FL_PLUS) && spec.flags & FL_SPACE)
		writer_char(buff, ' ');
	writer_signed(buff, i, spec.flags & FL_PLUS, pad_len + digits);
	if (spec.flags & FL_MINUS && spec.width != -1)
		writer_padn(buff, ' ', spec.width - total_len);
}

void	printf_unsigned(va_list *args, t_fmt_spec spec, t_buffer *buff)
{
	size_t	i;
	int		pad_len;
	int		total_len;
	int		digits;

	if (spec.t != FMT_UNSIGNED)
		return ;
	i = fetch_unsigned(args, spec.length);
	spec.flags &= ~FL_PLUS;
	spec.flags &= ~FL_SPACE;
	spec = normilize_int_spec(spec, (ssize_t)(i != 0));
	digits = unsigned_num_digits(i);
	pad_len = 0;
	if (spec.precision > digits)
		pad_len = spec.precision - digits;
	total_len = digits + pad_len;
	if (spec.flags & FL_MINUS)
		writer_unsigned(buff, i, digits + pad_len);
	if (spec.width != -1)
		writer_padn(buff, ' ', spec.width - total_len);
	if (!(spec.flags & FL_MINUS))
		writer_unsigned(buff, i, digits + pad_len);
}
