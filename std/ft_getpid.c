/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getpid.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 19:12:17 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/05 19:20:26 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ft_stdlib.h"
# include "ds.h"
# include "ft_stdio.h"
# include "ft_string.h"
# include "ft_stddef.h"

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
		return(0);
	}
	dyn_str_init(&file);
	dyn_str_append_fd(fd, &file);
	close(fd);
	temp = ft_split(file.buff, SPACE_CHAR);
	free(file.buff);
	fre(file.buff);
	ret = ft_strdup(temp[0]);
	free_tab(temp);
	return (ret);
}
