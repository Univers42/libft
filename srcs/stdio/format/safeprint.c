/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safeprint.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/04 18:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/09/04 18:00:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** Allocation-free formatted output.
**
** The ordinary printf family formats into a heap buffer (writer_init ->
** fn_malloc), which rules it out in three situations:
**
**   - inside a signal handler, where malloc is not async-signal-safe and
**     will deadlock if the interrupted code already held the allocator's
**     lock -- the classic case being a SIGSEGV caused by heap corruption;
**   - inside a malloc interposer loaded with LD_PRELOAD, where calling
**     malloc re-enters the very allocator being instrumented;
**   - anywhere an allocation failure must not turn into lost diagnostics.
**
** These entry points format into memory the caller already owns, so they
** allocate nothing, take no locks, and are safe in all three cases. Output
** longer than the buffer is truncated, and the return value still reports
** the length the result would have had -- the same contract as snprintf.
*/

#include "format.h"
#include "../output/output.h"
#include <unistd.h>

/*
** Prepares a t_buffer that writes into caller-owned memory. 'size' counts
** the terminating NUL, so the usable capacity is one less.
*/
int	writer_init_fixed(t_buffer *ret, char *buf, size_t size)
{
	*ret = (t_buffer){0};
	if (!buf || size == 0)
		return (1);
	ret->buff = buf;
	ret->cap = (int)(size - 1);
	ret->fixed = 1;
	ret->no_write = 1;
	ret->out_fd = -1;
	ret->buff[0] = 0;
	return (0);
}

int	ft_vsnprintf_safe(char *buf, size_t size, const char *str, va_list args)
{
	t_fmt_spec	spec;
	t_buffer	buff;
	va_list		ap;
	int			i;

	if (!str || writer_init_fixed(&buff, buf, size))
		return (0);
	i = 0;
	va_copy(ap, args);
	while (str[i])
	{
		spec = parse(str, &i, &ap);
		print_spec(&ap, spec, &buff);
	}
	va_end(ap);
	return (buff.total_put_in);
}

int	ft_snprintf_safe(char *buf, size_t size, const char *str, ...)
{
	va_list	args;
	int		ret;

	va_start(args, str);
	ret = ft_vsnprintf_safe(buf, size, str, args);
	va_end(args);
	return (ret);
}

/*
** Formats into a stack buffer and writes it in one call. The direct
** replacement for dprintf() on any path where allocating is unsafe.
*/
int	ft_fdprintf_safe(int fd, const char *str, ...)
{
	char	buf[FT_SAFE_BUFSIZE];
	va_list	args;
	int		len;

	va_start(args, str);
	len = ft_vsnprintf_safe(buf, sizeof(buf), str, args);
	va_end(args);
	if (len <= 0)
		return (0);
	if ((size_t)len > sizeof(buf) - 1)
		len = (int)(sizeof(buf) - 1);
	ft_write(fd, buf, (size_t)len);
	return (len);
}
