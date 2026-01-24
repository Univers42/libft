/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 03:24:17 by dlesieur          #+#    #+#             */
/*   Updated: 2026/01/24 14:45:15 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPE_H
# define PIPE_H

# include <fcntl.h>
# include <unistd.h>
# include "../../../include/ft_memory.h"
# include "../../../include/ft_string.h"

typedef struct s_pipe_fds
{
	int	r;
	int	w;
}	t_pipe_fds;

typedef struct s_redir_pair
{
	int	outfd;
	int	next_infd;
}	t_redir_pair;

/**
 * Create a pipe:
 * @return t_pipe_fds* is allocatd with st_alloc() from lifoba
 * on error returns NULL and errno is set.
 */
t_pipe_fds	*pipe_create(void);

/**
 * Close both ends (does not free the arena allocation)
 */
void		pipe_close_pair(t_pipe_fds *p);
/**
 * Close a single end: end == 0 => r, end == 1 => w
 */
void		pipe_close_end(t_pipe_fds *p, int end);
/**
 * Robust I/O helpers
 */
ssize_t		write_all(int fd, const void *buf, size_t n);
ssize_t		read_all(int fd, void *buf, size_t n);
/**
 * Read one line (up to and including '\\n') from fd into the arena and return
 * a nul-terminated buffer allocated/committed in the arena
 * On immediate EOF returns NULL.
 * !Caller must manage arean lifetime (pop_stack_mark)
 */
char		*read_line_arena(int fd);
/**
 * FD helpers
 */
int			set_nonblock(int fd, int enable);
int			set_cloexec(int fd, int enable);

#endif
