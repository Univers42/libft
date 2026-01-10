/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tee.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 23:42:08 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/19 03:19:01 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h> /* ft_strlen */
#include <fcntl.h>	/* open */
#include <stdio.h>	/* perror */

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
** ft_tee - simple line-based "tee" implementation (no zero-copy).
** fd_in, fd_out: file descriptors
** len: maximum number of bytes to duplicate
**
** Returns number of bytes duplicated on success, -1 on error and sets errno.
*/
ssize_t ft_tee(int fd_in, int fd_out, size_t len)
{
	ssize_t total;
	char *line;
	ssize_t l;

	total = 0;
	while (total < (ssize_t)len)
	{
		line = get_next_line(fd_in);
		if (!line)
			break;
		l = (ssize_t)strlen(line);
		if (total + l > (ssize_t)len)
			l = (ssize_t)len - total;
		if (write(fd_out, line, (size_t)l) < 0)
		{
			free(line);
			return (-1);
		}
		total += l;
		free(line);
	}
	return (total);
}

/*
** Small local test; marked weak so it doesn't interfere with real programs.
*/
// __attribute__((weak)) int main(void)
// {
// 	int input_fd;
// 	ssize_t n;

// <<<<<<< HEAD
// 	input_fd = open("input.txt", O_RDONLY | O_CREAT | O_TRUNC, 0644);
// 	if (input_fd == -1)
// 	{
// 		perror("open input (create)");
// 		return (1);
// 	}
// 	if (write(input_fd, "Line 1\nLine 2\nLine 3\n", 22) != 22)
// 	{
// 		perror("write input");
// 		close(input_fd);
// 		return (1);
// 	}
// 	close(input_fd);
// 	input_fd = open("input.txt", O_RDONLY);
// 	if (input_fd == -1)
// 	{
// 		perror("open input (read)");
// 		return (1);
// 	}
// 	n = ft_tee(input_fd, STDOUT_FILENO, 1024);
// 	if (n < 0)
// 		perror("ft_tee");
// 	close(input_fd);
// 	return (0);
// }
// =======
// 	input_fd = open("input.txt", O_RDONLY | O_CREAT | O_TRUNC, 0644);
// 	if (input_fd == -1)
// 	{
// 		perror("open input (create)");
// 		return (1);
// 	}
// 	if (write(input_fd, "Line 1\nLine 2\nLine 3\n", 22) != 22)
// 	{
// 		perror("write input");
// 		close(input_fd);
// 		return (1);
// 	}
// 	close(input_fd);
// 	input_fd = open("input.txt", O_RDONLY);
// 	if (input_fd == -1)
// 	{
// 		perror("open input (read)");
// 		return (1);
// 	}
// 	n = ft_tee(input_fd, STDOUT_FILENO, 1024);
// 	if (n < 0)
// 		perror("ft_tee");
// 	close(input_fd);
// 	return (0);
// }
// >>>>>>> hotfix
