/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dyl-syzygy <dyl-syzygy@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 20:47:24 by dyl-syzygy        #+#    #+#             */
/*   Updated: 2025/02/28 20:49:20 by dyl-syzygy       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_print_hex(unsigned char *p, unsigned int size, unsigned long addr)
{
	unsigned int	j;
	char			*hex;

	hex = "0123456789abcdef";
	ft_putnbr_base(addr, "0123456789abcdef");
	ft_putstr_fd(": ", STDOUT_FILENO);
	j = 0;
	while (j < 16 && j < size)
	{
		ft_putchar_fd(hex[p[j] / 16], STDOUT_FILENO);
		ft_putchar_fd(hex[p[j] % 16], STDOUT_FILENO);
		if (j % 2)
			ft_putchar_fd(' ', STDOUT_FILENO);
		j++;
	}
	while (j < 16)
	{
		ft_putstr_fd("   ", STDOUT_FILENO);
		if (j % 2)
			ft_putchar_fd(' ', STDOUT_FILENO);
		j++;
	}
}
