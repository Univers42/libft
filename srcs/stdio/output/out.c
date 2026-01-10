/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   out.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 16:51:10 by syzygy            #+#    #+#             */
/*   Updated: 2026/01/10 21:06:32 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "output.h"
#include "../format/format.h"
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
