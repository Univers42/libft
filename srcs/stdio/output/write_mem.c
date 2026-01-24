/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_mem.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 23:40:30 by dlesieur          #+#    #+#             */
/*   Updated: 2026/01/24 00:34:39 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "output.h"
#include "ft_string.h"
#include "ft_memory.h"

void	ft_fdputmem(int fd, char *s, int n)
{
	int	i;
	int	written;

	written = 0;
	while (written < n)
	{
		i = write(fd, s + written, n - written);
		if (i <= 0)
			break ;
		written += i;
	}
}

void	ft_putmem(char *s, int n)
{
	ft_fdputmem(1, s, n);
}

void	ft_eputmem(char *s, int n)
{
	ft_fdputmem(2, s, n);
}
