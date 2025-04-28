/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_format.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 07:31:03 by dlesieur          #+#    #+#             */
/*   Updated: 2025/03/24 21:25:44 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printf_p(void *p)
{
	char	*str;
	int		len;

	if (!p)
		return (ft_putstr_fd("(nil)", STDOUT_FILENO), 5);
	str = ft_itoa_base((unsigned long long)p, 16);
	if (!str)
		return (0);
	(ft_putstr_fd("0x", STDOUT_FILENO), ft_putstr_fd(str, STDOUT_FILENO));
	len = (int)ft_strlen(str) + 2;
	return (free(str), len);
}

int	ft_printf_x(unsigned int n)
{
	char	*str;
	int		len;

	str = ft_itoa_base(n, 16);
	if (!str)
		return (0);
	ft_putstr_fd(str, STDOUT_FILENO);
	len = (int)ft_strlen(str);
	return (free(str), len);
}

int	ft_printf_upper_x(unsigned int n)
{
	char	*str;
	int		len;

	str = ft_itoa_base(n, 16);
	if (!str)
		return (0);
	(ft_strtoupper(str), ft_putstr_fd(str, STDOUT_FILENO));
	len = (int)ft_strlen(str);
	return (free(str), len);
}

int	ft_printf_u(unsigned int n)
{
	char	*str;
	int		len;

	str = ft_utoa(n);
	if (!str)
		return (0);
	ft_putstr_fd(str, STDOUT_FILENO);
	len = (int)ft_strlen(str);
	return (free(str), len);
}

// Modified to return number of characters printed
int	ft_printf_d(int n)
{
	char	*str;
	int		len;

	if (n == INT_MIN)
		return (ft_printf_s("-2147483648"));
	str = ft_itoa(n);
	if (!str)
		return (0);
	len = (int)ft_strlen(str);
	ft_putstr_fd(str, STDOUT_FILENO);
	return (free(str), len);
}
