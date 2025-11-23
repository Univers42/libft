/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flushout.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 16:53:20 by syzygy            #+#    #+#             */
/*   Updated: 2025/11/23 17:00:33 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "output.h"

void    flush_all(void)
{
    flushout(&output);
    if (flusherr)
        flushout(&errout);
}

/**
 * The function writes the contents directly from the
 * internal buffer(dst->buf) to the underlying file descriptor
 * resetting nextc before the write is fine becase ft_write reads from
 * dst->buf and uses the separately computed len; resetting nextc only
 * affects futur buffering logic, not the bytes already in memory.
 * SUMMARY: floushout sends the pending buffer bytes to the FD
 * marks the buffer empty immediately, and reports errors via flags
 * ft_write handles the low-level robustness
 */
void    flushout(t_out *dst)
{
    const size_t  len = dst->nextc - dst->buf;

    if (!len || dst->fd < 0)
        return ;
    dst->nextc = dst->buf;
    if ((ft_write(dst->fd, dst->buf, len)))
        dst->flags |= OUTPUT_ERR;
}