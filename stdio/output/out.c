/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   out.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 16:51:10 by syzygy            #+#    #+#             */
/*   Updated: 2025/11/23 16:51:41 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "output.h"
#include <stdarg.h>

void    out1fmt(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	doformat(out1, fmt, ap);
	va_end(ap);
}