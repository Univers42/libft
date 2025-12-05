/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dyn_str_append_fd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 20:48:58 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/05 21:09:29 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ds.h"
#include "ft_stdio.h"

void    dyn_str_append_fd(int fd, t_dyn_str *ret)
{
	char	buff[1024];
	int		len;

	while (ST_SCANNING)
	{
		len = read(fd, buff, sizeof(buff));
		if (len == 0)
			break ;
		if (len > 0)
			dyn_str_pushnstr(ret, buff, len);
		else
			ft_eprintf("ctx :%s", read);
	}
}

