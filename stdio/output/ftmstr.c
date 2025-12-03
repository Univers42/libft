/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftmstr.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 14:59:14 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/03 23:42:35 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "output.h"
#include "format.h"

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
