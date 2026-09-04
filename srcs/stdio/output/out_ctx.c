/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   out_ctx.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/04 17:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/09/04 17:00:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** Single shared definition of the t_out context.
**
** This used to live in output.h as a "static inline" function holding a
** function-local static. Because each translation unit that included the
** header got its own copy of that static, outmem() could fill one context
** while flush_all() emptied a different one -- output could sit in a buffer
** that nothing was ever able to flush. Defining it once here gives every
** caller the same context, which is what the code always assumed.
*/

#include "output.h"

t_out_ctx	*get_outs(void)
{
	static t_out_ctx	ctx;
	static int			inited = 0;

	if (inited)
		return (&ctx);
	ctx.output.buf_size = OUTBUFSIZE;
	ctx.output.fd = STDOUT_FILENO;
	ctx.prev_err_out.buf_size = OUTBUFSIZE;
	ctx.prev_err_out.fd = STDERR_FILENO;
	ctx.errout.buf_size = OUTBUFSIZE;
	ctx.errout.fd = STDERR_FILENO;
	ctx.memout.buf_size = OUTBUFSIZE;
	ctx.memout.fd = STDOUT_FILENO;
	ctx.out1 = &ctx.output;
	ctx.out2 = &ctx.errout;
	inited = 1;
	return (&ctx);
}
