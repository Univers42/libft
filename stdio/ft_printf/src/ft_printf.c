/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 00:45:23 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/14 16:29:36 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "writer.h"
#include <stdarg.h>
#include <limits.h>

int	ft_vfprintf(int fd, const char *format, va_list *ap)
{
	static t_writer			buf_out;
	int						return_value;
	t_parser				parser;

	if (format == NULL || fd < 0)
		return (-1);
	/* Initialize the writer explicitly in FD mode */
	writer_init_fd(&buf_out, fd);
	return_value = parser_parse_and_write(&parser, format, ap, &buf_out);
	return (return_value);
}

int	ft_fprintf(int fd, const char *format, ...)
{
	va_list	ap;
	int		return_value;

	va_start(ap, format);
	return_value = ft_vfprintf(fd, format, &ap);
	va_end(ap);
	return (return_value);
}

int	ft_printf(const char *format, ...)
{
	va_list	ap;
	int		return_value;

	va_start(ap, format);
	return_value = ft_vfprintf(1, format, &ap);
	va_end(ap);
	return (return_value);
}

/* snprintf-style: cap counts the full destination size including the NUL.
   Returns number of chars written (excluding NUL). On truncation, returns
   the number of chars actually placed; sets writer error internally. */
int	ft_vsnprintf(char *dst, size_t cap, const char *format, va_list *ap)
{
	t_writer	buf_out;
	t_parser	parser;
	int			ret;

	if (!dst && cap != 0)
		return (-1);
	/* Reserve space for the trailing NUL by limiting data capacity */
	if (cap > 0)
		writer_init_buf(&buf_out, dst, cap - 1);
	else
		writer_init_buf(&buf_out, dst, 0);
	ret = parser_parse_and_write(&parser, format, ap, &buf_out);
	/* finalize with NUL (best-effort) */
	writer_terminate_cstr(&buf_out);
	if (ret < 0)
		return (ret);
	return (buf_out.n_written);
}

int	ft_snprintf(char *dst, size_t cap, const char *format, ...)
{
	va_list	ap;
	int		return_value;

	va_start(ap, format);
	return_value = ft_vsnprintf(dst, cap, format, &ap);
	va_end(ap);
	return (return_value);
}

/* sprintf-style: unbounded; caller must ensure dst has enough space.
   Implemented via vsnprintf with a very large capacity. */
int	ft_vsprintf(char *dst, const char *format, va_list *ap)
{
	/* give a huge capacity; user must ensure dst is large enough */
	return (ft_vsnprintf(dst, (size_t)-1, format, ap));
}

int	ft_sprintf(char *dst, const char *format, ...)
{
	va_list	ap;
	int		return_value;

	va_start(ap, format);
	return_value = ft_vsnprintf(dst, (size_t)-1, format, &ap);
	va_end(ap);
	return (return_value);
}
