/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fdprintf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 15:17:10 by syzygy            #+#    #+#             */
/*   Updated: 2025/11/21 15:17:54 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "format.h"
# include <stdarg.h>
# include <limits.h>

int ft_dprintf(int fd, const char *format, ...)
{
	va_list ap;
	int return_value;

	va_start(ap, format);
	return_value = ft_vdprintf(fd, format, &ap);
	va_end(ap);
	return (return_value);
}
