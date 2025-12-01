/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grab.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 19:32:38 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/01 15:16:51 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lifoba.h"
#include <string.h>

void ungrab_stack_str(void *s)
{
	st_unalloc(s);
}

char **grab_stack_str(char *p)
{
	char *base;

	base = stack_block();
	if (p >= base)
	{
		size_t bytes = (size_t)(p - base);
		size_t count = bytes / sizeof(char *);
		char **arr;

		/* allocate heap-backed array so callers may free() it */
		arr = (char **)arena_malloc((count + 1) * sizeof(char *));
		/* copy the pointer entries and null-terminate */
		memcpy(arr, base, count * sizeof(char *));
		arr[count] = NULL;
		return (arr);
	}
	return (NULL);
}

void grab_stack_block(size_t len)
{
	(void)st_alloc(len);
}
