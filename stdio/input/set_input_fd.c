/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_input_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 23:01:17 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/23 23:03:22 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"
#include <stdlib.h>

/*
** setinputfd - set parsefile fd and allocate buffer if needed.
** Simplified: allocate buffer for parsefile.
*/

void setinputfd(int fd, int push)
{
    t_input *in;

    (void)push;
    in = get_input();
    in->parsefile->fd = fd;
    in->parsefile->buf = (char *)malloc(BUFSIZ + PUNGETC_MAX + 1);
    in->parsefile->nextc = in->parsefile->buf;
    in->parsefile->nleft = 0;
}
