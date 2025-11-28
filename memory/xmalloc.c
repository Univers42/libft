/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xmalloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 19:31:01 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/28 19:52:23 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_memory.h"
#include <errno.h>
#include <stdio.h>

void	*xmalloc(size_t nbytes)
{
	void *ptr;

	ptr = malloc(nbytes);
	return (check_null(ptr));
}

void	*xrealloc(void *p, size_t old, size_t new)
{
    void	*nptr;
    size_t	copy;

    if (new == 0)
        return (xfree(p), NULL);
    nptr = xmalloc(new);
    if (nptr && p && old)
    {
		if (old < new)
			copy = old;
		else
			copy = new;
        ft_memcpy(nptr, p, copy);
    }
    return (xfree(p), check_null(nptr));
}

void	xfree(void *ptr)
{
	if (!ptr)
		return ;
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
