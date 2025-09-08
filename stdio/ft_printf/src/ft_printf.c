/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 00:45:23 by dlesieur          #+#    #+#             */
/*   Updated: 2025/09/08 17:17:55 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "parser_private.h"
#include "writer.h"
#include <stdarg.h>
#include <limits.h>

int	ft_vfprintf(int fd, const char *format, va_list *ap);

int ft_printf(const char *format, ...)
{
	va_list ap;
	int return_value;

	va_start(ap, format);
	return_value = ft_vfprintf(1, format, &ap);
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

// Enhanced log_print: accepts file for context
int log_print(t_log *state, const char *file, const char *format, ...)
{
	va_list ap;
	int ret = 0;
	if (!state || !file || !format || state->fd < 0)
		return -1;
	if (state->state == STATE_WARNING || state->state == STATE_FAILURE)
		ret += ft_fprintf(state->fd, "[%s] %s -> ", log_state_str(state->state), file);
	else if (state->state == STATE_INFO || state->state == STATE_SUCCESS)
		ret += ft_fprintf(state->fd, "[%s] ", log_state_str(state->state));
	else
		ret += ft_fprintf(state->fd, "[LOG] ");
	va_start(ap, format);
	ret += ft_vfprintf(state->fd, format, &ap);
	va_end(ap);
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
