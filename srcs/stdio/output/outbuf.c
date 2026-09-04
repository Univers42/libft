/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   outbuf.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/04 17:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/09/04 17:00:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** Buffered, thread-safe standard output.
**
** ft_printf used to hand every call straight to write(), one syscall per
** call. That is correct but slow, and it also means libft output can never
** interleave predictably with anything that does buffer. This file gives
** stdout the same discipline the C library uses:
**
**   - a terminal gets LINE buffering, so output still appears as it is
**     produced;
**   - a pipe or a redirect to a file gets FULL buffering, which collapses
**     thousands of small writes into one syscall per bufferful.
**
** The mode is never hardcoded: it is decided on first use from isatty(),
** and ft_setvbuf_mode() overrides it afterwards. stderr and ft_fdprintf()
** stay unbuffered, exactly as stderr and dprintf() are in the C library, so
** diagnostics are never lost when a program dies. Writing to stderr also
** flushes stdout first (see ft_veprintf), so the two still interleave in
** program order when both are redirected to the same file -- something the
** C library does not do.
**
** Everything is serialised by one mutex, so a single ft_printf() call is
** atomic with respect to other threads: two threads printing at once can no
** longer tear each other's lines apart.
*/

#include "output.h"
#include <pthread.h>
#include <unistd.h>

typedef struct s_stream
{
	int		fd;
	size_t	len;
	int		mode;
	int		ready;
	char	buf[OUTBUFSIZE];
}	t_stream;

static pthread_mutex_t	g_out_lock = PTHREAD_MUTEX_INITIALIZER;

static t_stream	*out_stream(void)
{
	static t_stream	stream = {STDOUT_FILENO, 0, FT_IOFBF, 0, {0}};

	return (&stream);
}

/*
** Picks the buffering discipline the first time the stream is written to.
** Driven by isatty() so that nothing depends on a compiled-in assumption
** about where stdout happens to point.
*/
static void	stream_ready(t_stream *s)
{
	if (s->ready)
		return ;
	if (isatty(s->fd))
		s->mode = FT_IOLBF;
	else
		s->mode = FT_IOFBF;
	s->ready = 1;
}

/* Caller must hold g_out_lock. */
static void	stream_flush(t_stream *s)
{
	size_t	len;

	len = s->len;
	if (len == 0)
		return ;
	s->len = 0;
	ft_write(s->fd, s->buf, len);
}

/* Caller must hold g_out_lock. Flushes as often as the buffer fills up. */
static void	stream_append(t_stream *s, const char *p, size_t len)
{
	size_t	room;

	while (len > 0)
	{
		if (s->len == sizeof(s->buf))
			stream_flush(s);
		room = sizeof(s->buf) - s->len;
		if (room > len)
			room = len;
		ft_memcpy(s->buf + s->len, p, room);
		s->len += room;
		p += room;
		len -= room;
	}
}

static int	has_newline(const char *p, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		if (p[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

/*
** Single entry point used by the printf family. Anything that is not stdout
** is written straight through, which keeps stderr and ft_fdprintf()
** unbuffered.
*/
int	ft_out_write(int fd, const char *p, size_t len)
{
	t_stream	*s;

	if (!p || len == 0)
		return (0);
	s = out_stream();
	if (fd != s->fd)
		return (ft_write(fd, p, len));
	pthread_mutex_lock(&g_out_lock);
	stream_ready(s);
	if (s->mode == FT_IONBF)
	{
		stream_flush(s);
		pthread_mutex_unlock(&g_out_lock);
		return (ft_write(fd, p, len));
	}
	stream_append(s, p, len);
	if (s->mode == FT_IOLBF && has_newline(p, len))
		stream_flush(s);
	pthread_mutex_unlock(&g_out_lock);
	return (0);
}

void	ft_fflush(int fd)
{
	t_stream	*s;

	s = out_stream();
	if (fd != s->fd && fd != -1)
		return ;
	pthread_mutex_lock(&g_out_lock);
	stream_flush(s);
	pthread_mutex_unlock(&g_out_lock);
}

/* mode is one of FT_IONBF, FT_IOLBF, FT_IOFBF. */
void	ft_setvbuf_mode(int mode)
{
	t_stream	*s;

	if (mode != FT_IONBF && mode != FT_IOLBF && mode != FT_IOFBF)
		return ;
	s = out_stream();
	pthread_mutex_lock(&g_out_lock);
	stream_flush(s);
	s->mode = mode;
	s->ready = 1;
	pthread_mutex_unlock(&g_out_lock);
}

/*
** fork() copies the buffer along with the rest of the address space, so
** anything still pending would be emitted twice: once by the parent and
** once by the child. Flushing in the prepare handler empties it first, and
** holding the lock across the fork guarantees no child ever inherits it
** locked by a thread that does not exist any more.
*/
static void	out_atfork_prepare(void)
{
	pthread_mutex_lock(&g_out_lock);
	stream_flush(out_stream());
}

static void	out_atfork_parent(void)
{
	pthread_mutex_unlock(&g_out_lock);
}

static void	out_atfork_child(void)
{
	out_stream()->len = 0;
	pthread_mutex_unlock(&g_out_lock);
}

__attribute__((constructor))
static void	ft_out_setup(void)
{
	pthread_atfork(out_atfork_prepare, out_atfork_parent, out_atfork_child);
}

/* Anything still buffered when the program ends must still be printed. */
__attribute__((destructor))
static void	ft_out_teardown(void)
{
	ft_fflush(STDOUT_FILENO);
}
