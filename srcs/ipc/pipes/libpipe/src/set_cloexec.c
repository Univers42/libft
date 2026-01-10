/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_cloexec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 03:06:44 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/19 03:06:45 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipe.h"

/* set or clear FD_CLOEXEC */
int set_cloexec(int fd, int enable)
{
    int flags;

    flags = fcntl(fd, F_GETFD, 0);
    if (flags < 0)
        return (-1);
    if (enable)
        flags |= FD_CLOEXEC;
    else
        flags &= ~FD_CLOEXEC;
    if (fcntl(fd, F_SETFD, flags) == 0)
        return (0);
    return (-1);
}
