/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/04 21:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/09/04 20:32:04 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** Ported from the standalone GetNextLine project, with everything that
** libft already provides taken out rather than repeated:
**
**   - its inline ft_memmove and ft_strnchr are libft's ft_memmove and
**     ft_memchr (byte-exact on a raw read() buffer, which is what matters);
**   - its ft_realloc(ptr, old, new) is libft's fn_realloc3 -- and is no
**     longer needed at all, because
**   - its t_dynstr + ensure_cap + append machinery IS libft's t_dyn_str:
**     dyn_str_pushnstr grows the buffer and NUL-terminates it in one call.
**
** What is genuinely GNL-specific stays here as statics: the per-fd read
** buffer, refilling it, and cutting a line out of it at the first '\n'.
**
** Three bugs in the original are fixed on the way. Its init() assigned the
** allocation to its own parameter, so the caller's slot stayed NULL and the
** next line dereferenced it (and it returned NULL from a void function). Its
** refill() read into the struct pointer rather than the buffer, which only
** worked because the buffer happened to be the first member. And its
** single-fd variant kept one static buffer for every descriptor, so reading
** two files alternately mixed their bytes. Per-fd state is allocated on
** first use and released at EOF, on error, and on allocation failure, so a
** file read to its end leaves nothing behind.
*/

#include "libft.h"
#include <unistd.h>

typedef struct s_gnl_fd
{
	char	buf[BUFFER_SIZE];
	char	*cur;
	char	*end;
}	t_gnl_fd;

static t_gnl_fd	**gnl_table(void)
{
	static t_gnl_fd	*table[GNL_FD_MAX];

	return (table);
}

/* Lazily allocates the reader state for fd. NULL if fd is out of range or
   the allocation fails. */
static t_gnl_fd	*gnl_state(int fd)
{
	t_gnl_fd	**table;

	if (fd < 0 || fd >= GNL_FD_MAX)
		return (NULL);
	table = gnl_table();
	if (!table[fd])
	{
		table[fd] = fn_malloc(sizeof(t_gnl_fd));
		if (!table[fd])
			return (NULL);
		table[fd]->cur = table[fd]->buf;
		table[fd]->end = table[fd]->buf;
	}
	return (table[fd]);
}

static void	gnl_release(int fd)
{
	t_gnl_fd	**table;

	table = gnl_table();
	fn_free(table[fd]);
	table[fd] = NULL;
}

/* One read() into the fd's buffer. Bytes read, 0 at EOF, -1 on error. */
static ssize_t	gnl_refill(t_gnl_fd *st, int fd)
{
	ssize_t	n;

	n = read(fd, st->buf, BUFFER_SIZE);
	if (n > 0)
	{
		st->cur = st->buf;
		st->end = st->buf + n;
	}
	return (n);
}

/* Appends from the buffer up to and including the first '\n', or all of it
   if there is none. 1 if a newline was consumed, 0 if not, -1 if the line
   could not grow. ft_memchr, not a strchr: the buffer is raw read() output
   and is not NUL-terminated. */
static int	gnl_take(t_gnl_fd *st, t_dyn_str *line)
{
	size_t	avail;
	size_t	chunk;
	char	*nl;

	avail = (size_t)(st->end - st->cur);
	nl = ft_memchr(st->cur, '\n', avail);
	chunk = avail;
	if (nl)
		chunk = (size_t)(nl - st->cur) + 1;
	if (dyn_str_pushnstr(line, st->cur, chunk) != ST_OK)
		return (-1);
	st->cur += chunk;
	return (nl != NULL);
}

/* A complete line, or the unterminated last line at EOF, is handed to the
   caller as is (the buffer becomes theirs). Anything else -- EOF with
   nothing pending, a read error, an allocation failure -- returns NULL and
   releases both the partial line and the fd's state, so nothing leaks. */
static char	*gnl_finish(int fd, t_dyn_str *line, int got, ssize_t n)
{
	if (got == 1 || (n == 0 && line->len > 0))
		return (line->buff);
	dyn_str_free(line);
	gnl_release(fd);
	return (NULL);
}

/*
** Frees the state kept for fd. A caller that stops before EOF -- a map error
** halfway through a file, a failed allocation on its own side -- has no
** other way to release the buffer get_next_line keeps for that descriptor,
** and torturette reports it as a leak of sizeof(t_gnl_fd) bytes inside
** get_next_line. Call it before close(). Harmless when nothing is pending.
*/
void	get_next_line_release(int fd)
{
	if (fd < 0 || fd >= GNL_FD_MAX)
		return ;
	gnl_release(fd);
}

char	*get_next_line(int fd)
{
	t_gnl_fd	*st;
	t_dyn_str	line;
	int			got;
	ssize_t		n;

	st = gnl_state(fd);
	if (!st || BUFFER_SIZE <= 0)
		return (NULL);
	dyn_str_init(&line);
	got = 0;
	n = 1;
	while (got == 0)
	{
		if (st->cur >= st->end)
		{
			n = gnl_refill(st, fd);
			if (n <= 0)
				break ;
		}
		got = gnl_take(st, &line);
	}
	return (gnl_finish(fd, &line, got, n));
}
