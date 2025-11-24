/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pread_fd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 23:00:39 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/23 23:03:24 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"
#include <unistd.h>
#include <errno.h>

/*
** preadfd - read into parsefile buffer from its fd
** Handles EINTR by retrying.
*/

int preadfd(void)
{
    t_input *in;
    char *buf;
    int fd;
    int nr;

    in = get_input();
    buf = in->parsefile->buf;
    fd = in->parsefile->fd;
    nr = (int)(BUFSIZ - input_get_lleft(in->parsefile));
    if (nr <= 0)
        return (0);
    for (;;)
    {
        ssize_t r = read(fd, buf + (in->parsefile->nextc - buf), (size_t)nr);
        if (r >= 0)
            return ((int)r);
        if (errno == EINTR)
            continue;
        return (-1);
    }
}
