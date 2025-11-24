/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pread_buffer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 23:00:35 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/23 23:03:24 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"
#include <string.h>

/*
** preadbuffer - copy leftover lleft into buffer head and fill rest by preadfd.
** Simplified: call preadfd() and update parsefile->nextc and lleft.
*/

int preadbuffer(void)
{
    t_input *in;
    int nr;
    int unget;
    char *buf;

    in = get_input();
    unget = (int)(in->parsefile->nextc - in->parsefile->buf);
    if (unget > PUNGETC_MAX)
        unget = PUNGETC_MAX;
    if (unget > 0)
        memmove(in->parsefile->buf, in->parsefile->nextc - unget, (size_t)unget);
    buf = in->parsefile->buf + unget;
    in->parsefile->nextc = buf;
    nr = preadfd();
    if (nr <= 0)
    {
        in->parsefile->nleft = 0;
        return (nr);
    }
    in->parsefile->nleft = nr;
    return (nr);
}
