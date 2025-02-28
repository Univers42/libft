/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_memory.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dyl-syzygy <dyl-syzygy@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 20:43:35 by dyl-syzygy        #+#    #+#             */
/*   Updated: 2025/02/28 20:44:46 by dyl-syzygy       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Prints the memory content of a given address.
 * @param addr The address to print
 * @param size The size of the memory to print
 * @return The address of the memory
 * @note This function prints the memory content of a given address
 * @note in a hexadecimal format.
 * @note EXAMPLE: ft_print_memory("Hello, World!", 13) prints:
 * @note 48 65 6c 6c 6f 2c 20 57 6f 72 6c 64 21
 * @note EXAMPLE: ft_print_memory("Hello, World!", 5) prints:
 * @note 48 65 6c 6c 6f
 * @note EXAMPLE: ft_print_memory("Hello, World!", 0) prints:
 */
void	print_hex(unsigned char *p, unsigned int size, unsigned long addr)
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

void	print_ascii(unsigned char *p, unsigned int size)
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

void	*ft_print_memory(void *addr, unsigned int size)
{
	unsigned int	i;
	unsigned char	*p;

	p = (unsigned char *)addr;
	i = 0;
	while (i < size)
	{
		print_hex(p + i, size - i, (unsigned long)(p + i));
		print_ascii(p + i, size - i);
		ft_putchar_fd('\n', STDOUT_FILENO);
		i += 16;
	}
	return (addr);
}
/*
int	main(void)
{
	char	*str;

	str = "Bonjour les amines\n\t
	ce qu on peut faire avec.\n\tprint_memory\n\nlol.lol\n ";
	ft_print_memory(str, 80);
	return (0);
}
*/