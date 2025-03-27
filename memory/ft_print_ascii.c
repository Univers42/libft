/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_ascii.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dyl-syzygy <dyl-syzygy@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 20:48:55 by dyl-syzygy        #+#    #+#             */
/*   Updated: 2025/02/28 20:49:54 by dyl-syzygy       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_print_ascii(unsigned char *p, unsigned int size)
{
	unsigned int	j;

	j = 0;
	while (j < 16 && j < size)
	{
		if (p[j] >= 32 && p[j] <= 126)
			ft_putchar_fd(p[j], STDOUT_FILENO);
		else
			ft_putchar_fd('.', STDOUT_FILENO);
		j++;
	}
}
