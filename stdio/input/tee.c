/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tee.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 23:42:08 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/03 23:42:09 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>

#if defined(__linux__)
#include <sys/syscall.h>
#ifndef SYS_tee
/* Some systems may not define SYS_tee in headers; define guard if needed. */
#if defined(__x86_64__)
#define SYS_tee 275
#elif defined(__i386__)
#define SYS_tee 323
#endif
#endif
#endif

/*
** ft_tee - thin wrapper around the Linux tee(2) syscall.
** fd_in, fd_out: file descriptors
** len: requested number of bytes to tee
** flags: currently passed through
**
** Returns number of bytes duplicated on success, -1 on error and sets errno.
** If the platform does not support tee, returns -1 and sets errno = ENOSYS.
*/

/**
 * runtime doesn't depend on kernel zero-copy
 * we want something simple and predictable for thsi function
 * 
 */
ssize_t ft_tee(int fd_in, int fd_out, size_t len, unsigned int flags)
{
	ssize_t	total;
	char	*line;
	ssize_t	l;

	total = 0;
	while (total < (ssize_t)len)
	{
		line = get_next_line(fd_in);
		if (!line)
			break ;
		l = strlen(line);
		if (total + l > (ssize_t)len)
			l = len - total;
		write(fd_out, line, l);
		total += l;
		free(line);
	}
	return (total);
}

__attribute__((weak))
int main(int argc, char **argv)
{
	int input_fd;
	char	*files[] = {"output1.txt", "output2.txt"};

	input_fd = open("input.txt", O_RDONLY | O_CREAT | O_TRUNC, 0644);
	write(input_fd, "Line 1\nLine 2\nLine 3\n", 22);
	close(input_fd);
	input_fd = open("input.txt", O_RDONLY);
	if (input_fd == -1)
	{
		perror("open input");
		return (1);
	}
	ft_tee(input_fd, STDOUT_FILENO, files, 2, 0);
	close(input_fd);
}