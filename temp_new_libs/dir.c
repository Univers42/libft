/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dir.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 19:45:45 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/14 00:11:29 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_lib.h"

char **ft_dir_content(char *dirpath, short flag)
{
	size_t i;
	DIR *dirp;
	struct dirent *d;
	char **file_list;
	size_t size;

	if (!(size = ft_size_dir(dirpath)))
		return (NULL);
	if (!(dirp = opendir(dirpath)))
		return (NULL);
	if (!(file_list = (char **)malloc(sizeof(char *) * (size + 1))))
		return (NULL);
	i = 0;
	while ((d = readdir(dirp)) && i < size)
		if (ft_dir_flag(flag, d->d_type))
			file_list[i++] = ft_strdup(d->d_name);
	file_list[i] = NULL;
	if (closedir(dirp) == -1)
		return (NULL);
	return (file_list);
}

short ft_dir_flag(short flag, unsigned char d_type)
{
	if (!flag)
		return (1);
	else if (flag == 1 && d_type != 4)
		return (1);
	else if (flag == 2 && d_type == 4)
		return (1);
	return (0);
}

char get_type(mode_t mode)
{
	if (S_ISLNK(mode))
		return ('l');
	else if (S_ISREG(mode))
		return ('-');
	else if (S_ISDIR(mode))
		return ('d');
	else if (S_ISCHR(mode))
		return ('c');
	else if (S_ISBLK(mode))
		return ('b');
	else if (S_ISFIFO(mode))
		return ('p');
	else if (S_ISSOCK(mode))
		return ('s');
	else
		return ('?');
}

char ft_file_type(char *path)
{
	struct stat buf;

	if (!path || !(*path))
		return (0);
	if (stat(path, &buf) == -1)
		return (0);
	return (get_type(buf.st_mode));
}

char *ft_read_file(char *path)
{
	int fd;
	char *content;
	char *buff;
	char *copy;
	char *tmp;

	if ((fd = open(path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR)) == -1)
		return (NULL);
	if (!(content = ft_strnew(1)))
	{
		close(fd);
		return (NULL);
	}
	while ((buff = get_next_line(fd)))
	{
		copy = content;
		tmp = ft_strjoin(copy, buff);
		if (!tmp)
		{
			ft_strdel(&buff);
			ft_strdel(&copy);
			close(fd);
			return (NULL);
		}
		content = tmp;
		ft_strdel(&buff);
		ft_strdel(&copy);
	}
	if ((close(fd)) == -1)
	{
		ft_strdel(&content);
		return (NULL);
	}
	return (content);
}

size_t ft_size_dir(char *dirpath)
{
	size_t size;
	DIR *dirp;

	size = 0;
	if (!dirpath)
		return (size);
	if (!(dirp = opendir(dirpath)))
		return (0);
	while (readdir(dirp))
		size++;
	if (closedir(dirp) == -1)
		return (0);
	return (size);
}

void ft_write_to_file(char *path, int flags, int perm, char *content)
{
	int fd;

	if ((fd = open(path, flags, perm)) == -1)
		return;
	ft_putstr_fd(content, fd);
	if ((close(fd)) == -1)
		return;
}
