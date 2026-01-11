/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_wrapper.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 02:10:30 by dlesieur          #+#    #+#             */
/*   Updated: 2026/01/11 00:16:56 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "format.h"
#include "../output/output.h"
#include <stdlib.h>

//int xvsnprintf(char *outbuf, size_t length, const char *fmt, va_list *ap)
//{
//	int ret;
//
//	intoff();
//	ret = ft_vsnprintf(outbuf, length, fmt, ap);
//	inton();
//	return (ret);
//}

// void	exwarning(const char *msg, va_list ap)
// {
// 	t_out	*errs;
// 	const char	*name;
// 	const char	*fmt;
// 	t_out_ctx	*ctx;

// 	ctx = get_outs();
// 	errs = ctx->out2;
// 	name = ternary(arg0, arg0, "sh");
// 	fmt = ternary(!command_name, "%s: %d: ", "%s: %d: %s: ");
// 	outfmt(errs, fmt, name, errlino, command_name);
// 	do_format(errs, msg, ap);
// #if FLUSHERR
// 	outc('\n', errs);
// #else
// 	outcslow('\n', errs);
// #endif
// }

// void exverror(int cond, const char *msg, va_list ap)
// {
// 	if (msg)
// 	{
// 		va_list aq;
// 		TRACE(("exverror(%d, \"", cond));
// 		va_copy(aq, ap);
// 		TRACEV((msg, aq));
// 		va_end(aq);
// 		TRACE(("\")pix=%d\n", getpid()));
// 	}
// 	else
// 	{
// 		TRACE(("exverror(%d, NULL) pid=%d\n", cond, getpid()));
// 	}
// 	/* if (msg) exvwarning(msg, ap); */
// 	flush_all();
// 	exraise(cond);
// }

// void sh_error(const char *msg, ...)
// {
// 	va_list ap;
// 	t_state_fd *log = get_state_fd();

// 	log->exit_status = 2;
// 	va_start(ap, msg);
// 	exverror(EXERROR, msg, ap);
// 	va_end(ap);
// 	abort();
// }
