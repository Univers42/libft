/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xmalloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 00:20:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/26 00:19:43 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "termcap.h"
#include <stdlib.h>
#include <unistd.h>

static void	memory_out(void)
{
	write(2, "virtual memory exhausted\n", 25);
	exit(1);
}

void	*xmalloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
		memory_out();
	return (ptr);
}

void	*xrealloc(void *ptr, size_t size)
{
	void	*new_ptr;

	new_ptr = realloc(ptr, size);
	if (!new_ptr)
		memory_out();
	return (new_ptr);
}

int	termcap_used_fallback(void)
{
	return (access_glob()->used_fallback);
}

void	termcap_mark_fallback(void)
{
	access_glob()->used_fallback = 1;
}
