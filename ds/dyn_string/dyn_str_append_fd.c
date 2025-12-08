/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dyn_str_append_fd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 20:48:58 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/08 02:05:35 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ds.h"
#include "ft_stdio.h"

int dyn_str_append_fd(int fd, t_dyn_str *out)
{
    char buf[4096];
    ssize_t r;

    if (!out)
        return (0);
    if (!out->buff)
        dyn_str_double(out);
    while ((r = read(fd, buf, sizeof(buf))) > 0)
    {
        if (!dyn_str_pushnstr(out, buf, (size_t)r))
            return (0);
    }
    return (r >= 0);
}