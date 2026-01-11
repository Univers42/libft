/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   putmem.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 00:10:45 by marvin            #+#    #+#             */
/*   Updated: 2026/01/11 00:10:45 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <unistd.h>

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
