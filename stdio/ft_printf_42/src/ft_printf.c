/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 07:38:59 by dlesieur          #+#    #+#             */
/*   Updated: 2025/03/24 21:31:40 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

// Modified to return 1 (one character printed)
int	ft_printf_c(int c)
{
	return (ft_putchar_fd((char)c, STDOUT_FILENO), 1);
}

// Modified to return string length
int	ft_printf_s(char *str)
{
	if (!str)
		return (ft_putstr_fd("(null)", STDOUT_FILENO), 6);
	return (ft_putstr_fd(str, STDOUT_FILENO), (int)ft_strlen(str));
}

// Modified to return 1 (one character printed)
int	ft_printf_percent(void)
{
	return (ft_printf_c('%'));
}

int	handle_conversion(const char *format, va_list args, int *count)
{
	if (*format == 'd' || *format == 'i')
		return (ft_printf_d(va_arg(args, int)));
	else if (*format == 'c')
		return (ft_printf_c(va_arg(args, int)));
	else if (*format == 's')
		return (ft_printf_s(va_arg(args, char *)));
	else if (*format == 'p')
		return (ft_printf_p(va_arg(args, void *)));
	else if (*format == 'x')
		return (ft_printf_x(va_arg(args, unsigned int)));
	else if (*format == 'X')
		return (ft_printf_upper_x(va_arg(args, unsigned int)));
	else if (*format == 'u')
		return (ft_printf_u(va_arg(args, unsigned int)));
	else if (*format == '%')
		return (ft_printf_percent());
	else
	{
		(ft_putchar_fd('%', 1), ft_putchar_fd(*format, 1));
		(*count) += 2;
		return (0);
	}
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		count;

	count = 0;
	if (!format)
		return (-1);
	va_start(args, format);
	while (*format)
	{
		if (*format == '%' && *(format + 1))
		{
			format++;
			count += handle_conversion(format, args, &count);
		}
		else
			(ft_putchar_fd(*format, 1), count++);
		format++;
	}
	return (va_end(args), count);
}
