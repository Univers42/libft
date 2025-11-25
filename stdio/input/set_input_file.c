/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_input_file.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 23:01:20 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/23 23:03:21 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"
#include <unistd.h>
#include <fcntl.h>

/*
** setinputfile - open file and set as current input.
** Minimal wrapper: call setinputfd after opening fd.
*/

int setinputfile(const char *fname, int flags)
{
    t_input *in;
    int fd;

    (void)flags;
    in = get_input();
    fd = open(fname, O_RDONLY);
    if (fd < 0)
        return (-1);
    in->parsefile->fd = fd;
    in->parsefile->nextc = in->parsefile->buf;
    in->parsefile->nleft = 0;
    return (fd);
}
