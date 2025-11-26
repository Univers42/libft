/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_snprintf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 15:17:05 by syzygy            #+#    #+#             */
/*   Updated: 2025/11/21 15:20:28 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "format.h"

int	ft_snprintf(char *dst, size_t cap, const char *format, ...)
{
	va_list	ap;
	int		return_value;

	va_start(ap, format);
	return_value = ft_vsnprintf(dst, cap, format, &ap);
	va_end(ap);
	return (return_value);
}
