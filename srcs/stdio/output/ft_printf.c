/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 15:18:29 by syzygy            #+#    #+#             */
/*   Updated: 2026/01/10 21:06:32 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../format/format.h"
#include <stdarg.h>
#include <unistd.h> /* STDOUT_FILENO */

int ft_printf(const char *format, ...)
{
	va_list ap;
	int return_value;

	va_start(ap, format);
	/* explicitly use STDOUT_FILENO to avoid ambiguity */
	return_value = ft_vdprintf(STDOUT_FILENO, format, &ap);
	va_end(ap);
	return (return_value);
}
