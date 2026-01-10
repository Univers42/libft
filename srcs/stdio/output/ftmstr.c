/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftmstr.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 14:59:14 by dlesieur          #+#    #+#             */
/*   Updated: 2026/01/10 21:05:36 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "output.h"
#include "../format/format.h"

int fmtstr(char *outbuf, size_t length, const char *fmt, ...)
{
	va_list ap;
	int ret;

	va_start(ap, fmt);
	ret = xvsnprintf(outbuf, length, fmt, &ap);
	va_end(ap);
	if (ret > (int)length)
		return (length);
	return (ret);
}
