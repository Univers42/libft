/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flush_tee.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 22:59:02 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/23 23:03:28 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"
#include <unistd.h>

/*
** flush_tee - read and discard 'pending' bytes from fd 0 into buf of size nr.
** Simple loop, retry on short reads. Does not use tee(2) or FILE*.
*/

void flush_tee(void *buf, int nr, int pending)
{
    ssize_t r;
    int toread;

    if (!buf || nr <= 0 || pending <= 0)
        return;
    while (pending > 0)
    {
        toread = (pending > nr) ? nr : pending;
        r = read(0, buf, (size_t)toread);
        if (r <= 0)
            break;
        pending -= (int)r;
    }
}
