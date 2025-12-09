/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line_arena.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 03:37:53 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/09 18:20:50 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_memory.h"
#include "pipe.h"

/*
 * helper: read bytes from fd, append into arena via st_putc.
 * returns:
 *   -1 on error (st_unalloc already called for immediate error),
 *    0 on immediate EOF with no data (st_unalloc already called),
 *    1 when finished reading (either newline seen or EOF after data)
 */
static int read_line_fill(int fd, char *base, char **pp)
{
	char buf;
	ssize_t r;

	while (1)
	{
		r = read(fd, &buf, 1);
		if (r < 0)
		{
			if (errno == EINTR)
				continue;
			return (st_unalloc(base), -1);
		}
		if (r == 0)
		{
			if (*pp == base)
				return (st_unalloc(base), 0);
			return (1);
		}
		*pp = st_putc((int)(unsigned char)buf, *pp);
		if (buf == '\n')
			return (1);
	}
}

/*
 * Read one line from fd into arena.
 * Uses start_stack_str() / st_putc() / stack_str_nul() / grab_stack_str()
 * to build and commit the string in the arena. Caller must ensure the arena
 * context remains valid for the returned string lifetime.
 */
char *read_line_arena(int fd)
{
	char *base;
	char *p;
	int r;

	base = start_stack_str();
	p = base;
	r = read_line_fill(fd, base, &p);
	if (r <= 0)
		return (NULL);
	p = stack_str_nul(p);
	grab_stack_str(p);
	return (base);
}
