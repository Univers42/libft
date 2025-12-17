/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flushout.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 16:53:20 by syzygy            #+#    #+#             */
/*   Updated: 2025/11/30 02:34:25 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "output.h"
#include <stddef.h>

void flush_all(void)
{
    t_out_ctx *ctx = get_outs();

    if (!ctx)
        return;
    /* flush primary output and error outputs */
    flushout(&ctx->output);
    flushout(&ctx->errout);
    /* also flush previous error buffer and memory buffer if present */
    flushout(&ctx->prev_err_out);
    flushout(&ctx->memout);
}

/**
 * Write pending bytes from dst->buf to dst->fd, reset buffer pointers.
 * Safe-guards: return early if dst or its buffer isn't initialized or nothing to write.
 */
void flushout(t_out *dst)
{
    size_t len;

    if (dst == NULL || dst->buf == NULL || dst->nextc == NULL)
        return;
    if (dst->nextc <= dst->buf || dst->fd < 0)
        return;
    len = (size_t)(dst->nextc - dst->buf);
    if (len == 0)
        return;
    /* reset nextc before write (caller logic relies on this) */
    dst->nextc = dst->buf;
    if (ft_write(dst->fd, dst->buf, len) != 0)
        dst->flags |= OUTPUT_ERR;
}
