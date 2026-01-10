/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stdin_clear_nonblock.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 23:01:28 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/23 23:01:29 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"
#include <fcntl.h>
#include <unistd.h>

/*
** Clear O_NONBLOCK on fd 0. Returns previous flags (or -1).
*/

int stdin_clear_nonblock(void)
{
    int flags;

    flags = fcntl(0, F_GETFL, 0);
    if (flags >= 0)
    {
        flags &= ~O_NONBLOCK;
        (void)fcntl(0, F_SETFL, flags);
    }
    return (flags);
}
