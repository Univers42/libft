/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_write.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 16:41:11 by syzygy            #+#    #+#             */
/*   Updated: 2025/11/23 16:48:11 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <errno.h>

/**
 * Wrapper around write that ensure all n bytes are written
 * handling partial writes, signals (EINTR) and very large sizes
 * by chopping into ssize_max sizes chunks returns 0 on success, -1 on error
 */
int ft_write(int fd, const void *p, size_t n)
{
    const char *buf = p;
    ssize_t i;
    size_t m;

    while (n) {
        m = n;
        if (m > SSIZE_MAX)
            m = SSIZE_MAX;
		i = write(fd, buf, m);
        while (i < 0 && errno == EINTR)
			i = write(fd, buf, m);
        if (i < 0)
            return (-1);
        buf += i;
        n -= (size_t)i;
    }
    return 0;
}