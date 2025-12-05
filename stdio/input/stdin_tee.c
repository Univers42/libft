/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stdin_tee.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 23:01:31 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/05 02:13:45 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

/*
** Minimal stdin_tee and flush_tee implementations.
** stdin_tee: if non-tty, try to create pipe pair in stdin_state.pip.
**           returns number of bytes tee'd or 0 on tty/failure.
** flush_tee: read and discard 'pending' bytes from fd 0 using provided buffer.
*/

int stdin_tee(void *buf, int nr)
{
    t_input *in;
    int err;

    in = get_input();
    (void)buf;
    (void)nr;
    if (in->stdin_istty)
        return (0);
    if (!in->stdin_state.pip[0])
    {
        err = pipe(in->stdin_state.pip);
        if (err < 0)
            return (-1);
    }
    in->stdin_state.pending = 0;
    return (0);
}

