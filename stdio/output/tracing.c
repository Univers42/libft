/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 18:09:34 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/23 18:28:39 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "output.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef struct s_file
{
	int				fd;
	int				flags;
	char			*io_read_ptr;
	char			*io_read_end;
	char			*io_read_base;
	char			*io_write_base;
	char			*io_write_ptr;
	char			*io_write_end;
	char			*io_buf_base;
	char			*io_buf_end;
	char			*io_save_base;
	char			*io_bak;
	char			*save_end;
	struct s_file	*chain;
	int				flags2;
	unsigned short	cur_column;
	signed char		vable_offset;
	char			short_buf[1];
	void			*lock;
}	t_file;

/*
** Singleton storage accessors (no file-scope globals).
** tracefd_ptr() and debug_ptr() return pointers to internal storage.
** get_/set_ wrappers are provided to use these values.
*/

static int	*tracefd_ptr(void)
{
	static int	fd_storage = 2;
	return (&fd_storage);
}

int	get_trace_fd(void)
{
	return (*tracefd_ptr());
}

void	set_trace_fd(int newfd)
{
	if (newfd >= 0)
		*tracefd_ptr() = newfd;
}

static int	*debug_ptr(void)
{
	static int	dbg = 0;
	return (&dbg);
}

int	get_debug(void)
{
	return (*debug_ptr());
}

void	set_debug(int newval)
{
	*debug_ptr() = newval ? 1 : 0;
}

/*
** Write indentation to the given fd. If pfx is non-NULL it is written
** before the last tab.
*/

void	indent(int amount, char *pfx, int fd)
{
	int		i;
	size_t	len;

	if (amount <= 0)
		return ;
	i = 0;
	while (i < amount)
	{
		if (pfx && i == amount - 1)
		{
			len = strlen(pfx);
			if (len > 0)
				(void)write(fd, pfx, len);
		}
		(void)write(fd, "\t", 1);
		i++;
	}
}

/*
** Tracing helpers use the singleton debug flag and trace fd.
*/

void	trputc(int c)
{
	char	ch;
	int		fd;

	if (get_debug() != 1)
		return ;
	ch = (char)c;
	fd = get_trace_fd();
	(void)write(fd, &ch, 1);
}

void	trace(const char *fmt, ...)
{
	va_list	va;
	char	buf[4096];
	int		n;
	int		fd;

	if (get_debug() != 1)
		return ;
	va_start(va, fmt);
	n = vsnprintf(buf, sizeof(buf), fmt, va);
	va_end(va);
	if (n > 0)
	{
		fd = get_trace_fd();
		(void)write(fd, buf, (size_t)n);
	}
}

void	tracev(const char *fmt, va_list va)
{
	char	buf[4096];
	int		n;
	int		fd;

	if (get_debug() != 1)
		return ;
	n = vsnprintf(buf, sizeof(buf), fmt, va);
	if (n > 0)
	{
		fd = get_trace_fd();
		(void)write(fd, buf, (size_t)n);
	}
}

void	trputs(const char *s)
{
	size_t	len;
	int		fd;

	if (get_debug() != 1 || s == NULL)
		return ;
	len = strlen(s);
	if (len > 0)
	{
		fd = get_trace_fd();
		(void)write(fd, s, len);
	}
}