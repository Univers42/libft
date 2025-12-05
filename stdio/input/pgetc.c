/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pgetc.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 23:00:12 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/05 00:41:04 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"

/* forward declaration to satisfy the compiler (implemented in pread_buffer.c) */
int preadbuffer(void);

/*
** pgetc using singleton
*/

int pgetc(void)
{
    t_input *in;

    in = get_input();
    if (in->parsefile->unget)
    {
        long unget = -(long)(unsigned)in->parsefile->unget--;
        return (int)in->parsefile->nextc[unget];
    }
    if (in->parsefile->nleft > 0)
    {
        in->parsefile->nleft--;
        return (unsigned char)*in->parsefile->nextc++;
    }
    if (preadbuffer() <= 0)
        return (PEOF);
    in->parsefile->nleft--;
    return (unsigned char)*in->parsefile->nextc++;
}
