/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 15:18:29 by syzygy            #+#    #+#             */
/*   Updated: 2025/11/21 15:19:10 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <format.h>

int ft_printf(const char *format, ...)
{
	va_list ap;
	int return_value;

	va_start(ap, format);
	return_value = ft_vdprintf(1, format, &ap);
	va_end(ap);
	return (return_value);
}