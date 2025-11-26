/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 15:21:26 by syzygy            #+#    #+#             */
/*   Updated: 2025/11/21 15:21:46 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "format.h"

int	ft_sprintf(char *dst, const char *format, ...)
{
	va_list	ap;
	int		return_value;

	va_start(ap, format);
	return_value = ft_vsnprintf(dst, (size_t)-1, format, &ap);
	va_end(ap);
	return (return_value);
}