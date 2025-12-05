/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 20:44:09 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/05 20:46:33 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_memory.h"
#include "ft_string.h"

int write_file(const char *str, int fd)
{
	int tot;
	int written;
	const int len = ft_strlen(str);

	tot = 0;
	while (tot != len)
	{
		written = write(fd, str + tot, len - tot);
		if (written < 0)
			return (1);
		tot += written;
	}
	return (0);
}
