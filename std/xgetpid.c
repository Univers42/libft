/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xgetpid.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 19:12:17 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/20 03:27:04 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_stdlib.h"
#include "ds.h"
#include "ft_stdio.h"
#include "ft_string.h"
#include "ft_stddef.h"

static void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		free(tab[i++]);
	free(tab);
}

char	*xgetpid(void)
{
	int			fd;
	t_dyn_str	file;
	char		*ret;
	char		**temp;

	fd = open("/proc/self/stat", O_RDONLY);
	if (fd < 0)
	{
		warning_error("Cannot get PID.");
		return (0);
	}
	dyn_str_init(&file);
	dyn_str_append_fd(&file, fd);
	close(fd);
	temp = ft_split(file.buff, SPACE_CHAR);
	free(file.buff);
	ret = ft_strdup(temp[0]);
	free_tab(temp);
	return (ret);
}
