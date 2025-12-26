/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dyn_str_write_file.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 00:20:28 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/27 00:24:16 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ds.h"

bool	dyn_str_write_file(t_dyn_str buff, char *fname)
{
	int	fd;
	size_t	written;
	int		ret;

	fd = open(fname, O_CREAT | O_TRUNC | O_WRONLY, 0777);
	if (fd < 0)
	{
		print_err();
		ft_eprintf("Openng the file for writing: %s: %s\n", fname, strerror(errno));
		return (false);
	}
	written = 0;
	while (1)
	{
		ret = write(fd, buff, buff.buff + written, buff.len - written);
		if (ret < 0)
			return (print_err(), ft_eprintf("Failed to write to file: %s: %s\n",
				fname, strerror(errno)), close(fd), false);
		if (ret <= 0)
				break ;
		written += ret;
		return (close(fd), true);
	}
}
