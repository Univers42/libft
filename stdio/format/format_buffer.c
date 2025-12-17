/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_buffer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 15:20:48 by syzygy            #+#    #+#             */
/*   Updated: 2025/12/01 01:33:27 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "format.h"

/* snprintf-style: cap counts the full destination size including the NUL.
 * Returns number of chars written (excluding NUL). On truncation, returns
 * the number of chars actually placed; sets writer error internally.
 * Reserve space for hte trailing NULL by limiting data capacity
 * Finalize with NULL (best-effort)
 */
int ft_vsnprintf(char *dst, size_t cap, const char *format, va_list *ap)
{
	t_writer buf_out;
	t_format_parser parser;
	int ret;

	if (!dst && cap != 0)
		return (-1);
	if (cap > 0)
		writer_init_buf(&buf_out, dst, cap - 1);
	else
		writer_init_buf(&buf_out, dst, 0);
	ret = parser_parse_and_write(&parser, format, ap, &buf_out);
	writer_terminate_cstr(&buf_out);
	if (ret < 0)
		return (ret);
	return (buf_out.n_written);
}

/* sprintf-style: unbounded; caller must ensure dst has enough space.
   Implemented via vsnprintf with a very large capacity.
   we need to give a huge capacity LONG_MAX
*/
int ft_vsprintf(char *dst, const char *format, va_list *ap)
{
	return (ft_vsnprintf(dst, (size_t)-1, format, ap));
}
