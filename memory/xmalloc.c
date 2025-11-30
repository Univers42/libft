/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xmalloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 19:31:01 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/30 20:03:40 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_memory.h"
#include <errno.h>
#include <stdio.h>

void *xmalloc(size_t nbytes)
{
    void *ptr;

    ptr = malloc(nbytes);
    return (check_null(ptr));
}

void *xrealloc(void *p, size_t old, size_t newsize)
{
    void *nptr;
    size_t copy;

    if (new == 0)
        return (xfree(p), NULL);
    nptr = xmalloc(newsize);
    if (nptr && p && old)
    {
        if (old < newsize)
            copy = old;
        else
            copy = newsize;
        ft_memcpy(nptr, p, copy);
    }
    return (xfree(p), check_null(nptr));
}

void xfree(void *ptr)
{
    if (!ptr)
        return;
    free(ptr);
}

// __attribute__((weak))
// int	main(void)
// {
// 	char *greeting;

// 	greeting = xmalloc(strlen("hello"));
// 	strcpy(greeting, "hello");
// 	return (0);
// }
