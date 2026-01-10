/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_snprintf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 15:17:05 by syzygy            #+#    #+#             */
/*   Updated: 2026/01/10 21:06:32 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../format/format.h"

int	ft_snprintf(char *dst, size_t cap, const char *format, ...)
{
	va_list	ap;
	int		return_value;

	va_start(ap, format);
	return_value = ft_vsnprintf(dst, cap, format, &ap);
	va_end(ap);
	return (return_value);
}
