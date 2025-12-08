/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 20:44:09 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/08 02:27:18 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_memory.h"
#include "ft_string.h"
#include "ft_stdio.h"

int write_file_buf(const char *buf, size_t len, int fd)
{
    size_t total = 0;
    ssize_t w;

    if (fd < 0 || !buf)
        return -1;
    while (total < len)
    {
        w = write(fd, buf + total, len - total);
        if (w < 0)
        {
            if (errno == EINTR)
                continue;
            return -1;
        }
        if (w == 0)
            break;
        total += (size_t)w;
    }
    return (total == len) ? 0 : -1;
}

/* Legacy wrapper for NUL-terminated strings */
int write_file(const char *str, int fd)
{
    if (!str)
        return -1;
    return write_file_buf(str, strlen(str), fd);
}