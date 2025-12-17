/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mktemp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 20:31:53 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/12 21:14:12 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_memory.h"
#include "ft_string.h"
#include "ft_stdlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <errno.h>
#include "ft_stdio.h"


#ifndef REAL_LIKE_MKTEMP

int		ft_mktemp(const char *prefix, const char *suffix, mode_t mode, char **out_path)
{
	
}
#else

int		ft_mktemp(const char *template, mode_t mode)
{
	size_t	len;
	int		x_count;
	int		fd;
	char	*x_start;
	pid_t	pid;

	len = ft_strlen(template);
	if (len < 6)
		return (-1);
	x_count = 0;
	while (len > 0 && template[len - 1 - x_count] == 'X')
		x_count++;
	if (x_count < 6)
		return (-1);
	x_start = template + (len - x_count);
	pid = ft_atoi(xgetpid());
	i = -1;
	while (++i < 10000)
	{
		snprintf(x_start, x_count + 1, "%0*u", x_count, (pid + i) % 1000000);
		fd = open(template, O_WRONLY | O_CREAT | O_EXCL, mode);
		if (fd >= 0)
			return (fd);
		if (errno != EEXIST)
			return (-1);
	}
	errno = EEXIST;
	return (-1);
}

#endif
void    randomize_template(char *template)
{
	const int len = ft_strlen(template);
	const char template_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	int x_count = 0;
	int i;

	i = len - 1;
	while (i >= 0 && template[i] == 'X')
	{
		x_count++;
		i--;
	}
	if (x_count < 3)
	{
		fprintf(stderr, "Template must end with at least XXX\n");
		exit(1);
	}
	ft_srand(time(NULL) ^ ft_atoi(xgetpid()), RNG_XORSHIFT);
	i = len - x_count;
	while (i < len)
	{
		template[i] = template_chars[ft_rand() % 62];
	}
}

int	main(int argc, char **argv)
{
	int	create_dir = 0;
	int	dry_run = 0;
	char	*template = NULL;
	char	*path;
	int		max_attempts;
	int		attempt;

	for (int i = 1; i < argc; i++)
	{
		if (ft_strcmp(argv[i], "-d") == 0)
			create_dir = 1;
		else if (strcmp(argv[i], "-U") == 0)
			dry_run = 1;
		else if (template == NULL)
			template = argv[i];
	}
	if (template == NULL)
		template = "/tmp/tmp.XXXXXXXXX";
	path = ft_strdup(template);
	if (!path)
	{
		perror("strdup");
		return (1);
	}
	max_attempts = 100;
	attempt = 0;
	while (attempt < max_attempts)
	{
		randomize_template(path);
		if (dry_run)
		{
			printf("%s\n", path);
			free(path);
			return (0);
		}
		if (create_dir)
		{
			if (mkdir(path, 0700) == 0)
			{
				printf("%s\n", path);
				free(path);
				return (0);
			}
			if (errno != EEXIST)
			{
				perror("mkdir");
				free(path);
				return (1);
			}
			else
			{
				int fd = open(path, O_RDWR | O_CREAT | O_EXCL, 0600);
				if (fd >= 0)
				{
					close(fd);
					printf("%s\n", path);
					free(path);
					return (0);
				}
				if (errno != EEXIST)
				{
					perror("open");
					free(path);
					return (1);
				}
			}
		}
		ft_strcpy(path, template);
		attempt++;
	}
	ft_dprintf(STDERR_FILENO, "FAiled to create unique temporary file after %d attempts\n", max_attempts);
	free(path);
	return (1);
}
