/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_memory.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 20:43:35 by dyl-syzygy        #+#    #+#             */
/*   Updated: 2025/03/23 06:13:33 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	print_hex(unsigned char *addr, unsigned int size);
void	print_ascii(unsigned char *addr, unsigned int size);

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
void	*ft_print_memory(void *addr, unsigned int size)
{
	unsigned int	i;
	unsigned char	*p;

	p = (unsigned char *)addr;
	i = 0;
	while (i < size)
	{
		print_hex(p + i, size - i);
		print_ascii(p + i, size - i);
		ft_putchar_fd('\n', STDOUT_FILENO);
		i += 16;
	}
	return (addr);
}

void	print_hex(unsigned char *addr, unsigned int size)
{
	unsigned int	i;

	i = 0;
	while (i < 16 && i < size)
	{
		if (i % 2 == 0)
			ft_putchar_fd(' ', STDOUT_FILENO);
		ft_putnbr_base(addr[i], "0123456789abcdef");
		i++;
	}
	while (i < 16)
	{
		if (i % 2 == 0)
			ft_putchar_fd(' ', STDOUT_FILENO);
		ft_putstr_fd("  ", STDOUT_FILENO);
		i++;
	}
	ft_putstr_fd("  ", STDOUT_FILENO);
}

void	print_ascii(unsigned char *addr, unsigned int size)
{
	unsigned int	i;

	i = 0;
	while (i < 16 && i < size)
	{
		if (addr[i] >= 32 && addr[i] <= 126)
			ft_putchar_fd(addr[i], STDOUT_FILENO);
		else
			ft_putchar_fd('.', STDOUT_FILENO);
		i++;
	}
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