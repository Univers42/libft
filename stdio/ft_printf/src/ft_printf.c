/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 00:45:23 by dlesieur          #+#    #+#             */
/*   Updated: 2025/09/08 15:55:11 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "parser_private.h"
#include "writer.h"
#include <stdarg.h>
#include <limits.h>

int ft_vfprintf(int fd, const char *format, va_list *ap)
{
	static t_writer buf_out;
	int return_value;
	t_parser parser;

	if (format == NULL || fd < 0)
		return (-1);
	/* Initialize the writer explicitly in FD mode */
	writer_init_fd(&buf_out, fd);
	return_value = parser_parse_and_write(&parser, format, ap, &buf_out);
	return (return_value);
}

int ft_fprintf(int fd, const char *format, ...)
{
	va_list ap;
	int return_value;

	va_start(ap, format);
	return_value = ft_vfprintf(fd, format, &ap);
	va_end(ap);
	return (return_value);
}

int ft_printf(const char *format, ...)
{
	va_list ap;
	int return_value;

	va_start(ap, format);
	return_value = ft_vfprintf(1, format, &ap);
	va_end(ap);
	return (return_value);
}

/* snprintf-style: cap counts the full destination size including the NUL.
   Returns number of chars written (excluding NUL). On truncation, returns
   the number of chars actually placed; sets writer error internally. */
int ft_vsnprintf(char *dst, size_t cap, const char *format, va_list *ap)
{
	t_writer buf_out;
	t_parser parser;
	int ret;

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

int ft_snprintf(char *dst, size_t cap, const char *format, ...)
{
	va_list ap;
	int return_value;

	va_start(ap, format);
	return_value = ft_vsnprintf(dst, cap, format, &ap);
	va_end(ap);
	return (return_value);
}

/* sprintf-style: unbounded; caller must ensure dst has enough space.
   Implemented via vsnprintf with a very large capacity. */
int ft_vsprintf(char *dst, const char *format, va_list *ap)
{
	/* give a huge capacity; user must ensure dst is large enough */
	return (ft_vsnprintf(dst, (size_t)-1, format, ap));
}

int ft_sprintf(char *dst, const char *format, ...)
{
	va_list ap;
	int return_value;

	va_start(ap, format);
	return_value = ft_vsnprintf(dst, (size_t)-1, format, &ap);
	va_end(ap);
	return (return_value);
}

// Helper to get log state string
static const char *log_state_str(t_state state)
{
	switch (state)
	{
	case STATE_WARNING:
		return "WARNING";
	case STATE_INFO:
		return "INFO";
	case STATE_SUCCESS:
		return "SUCCESS";
	case STATE_FAILURE:
		return "FAILURE";
	default:
		return "LOG";
	}
}

// Enhanced log_print: accepts file, line, func for context
// Usage: log_print(&log, __FILE__, __LINE__, __func__, "format...", ...);
int log_print(t_log *state, const char *file, int line, const char *func, const char *format, ...)
{
	va_list ap;
	int ret = 0;
	if (!state || !format || state->fd < 0)
		return -1;
	// Print prefix based on state
	if (state->state == STATE_WARNING || state->state == STATE_FAILURE)
		ret += ft_fprintf(state->fd, "[%s] [%s] [%s:%d] -> ", log_state_str(state->state), func, file, line);
	else if (state->state == STATE_INFO || state->state == STATE_SUCCESS)
		ret += ft_fprintf(state->fd, "[%s] ", log_state_str(state->state));
	else
		ret += ft_fprintf(state->fd, "[LOG] ");
	// Print user message
	va_start(ap, format);
	ret += ft_vfprintf(state->fd, format, &ap);
	va_end(ap);
	// Newline
	ret += ft_fprintf(state->fd, "\n");
	return ret;
}

int ft_vlogprintf(t_log *state, const char *format, va_list *ap)
{
	static t_writer buf_out;
	int return_value;
	t_parser parser;

	if (format == NULL || !state || state->fd < 0)
		return (-1);
	writer_init_fd(&buf_out, state->fd);
	return_value = parser_parse_and_write(&parser, format, ap, &buf_out);
	return (return_value);
}
