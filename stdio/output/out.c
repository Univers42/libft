/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   out.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 16:51:10 by syzygy            #+#    #+#             */
/*   Updated: 2025/12/03 23:42:41 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "output.h"
#include "format.h"
#include <stdarg.h>

void    out1fmt(const char *fmt, ...)
{
	va_list ap;
	t_out_ctx	*ctx;

	ctx = get_outs();
	va_start(ap, fmt);
	do_format(ctx->out1, fmt, ap);
	va_end(ap);
}
