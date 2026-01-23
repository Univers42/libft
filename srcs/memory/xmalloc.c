/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xmalloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 19:31:01 by dlesieur          #+#    #+#             */
/*   Updated: 2026/01/23 23:09:43 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_memory.h"
#include <errno.h>
#include <stdio.h>

void	*xmalloc(size_t nbytes)
{
	void	*ptr;

	ptr = malloc(nbytes);
	return (check_null(ptr));
}

void	*xrealloc(void *p, size_t old, size_t newsize)
{
	void	*nptr;
	size_t	copy;

	if (newsize == 0)
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

bool	check_overflow(size_t n, size_t size)
{
	if (size != 0 && n > SIZE_MAX / size)
		return (true);
	return (false);
}

void	*xcalloc(size_t nmemb, size_t size)
{
	void	*ptr;

	if (check_overflow(nmemb, size))
		return (NULL);
	ptr = malloc(nmemb * size);
	if (!ptr)
		return (NULL);
	ft_memset(ptr, 0, nmemb * size);
	return (ptr);
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
