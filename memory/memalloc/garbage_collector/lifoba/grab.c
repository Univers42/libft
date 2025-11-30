/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grab.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 19:32:38 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/29 18:49:55 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lifoba.h"

void	ungrab_stack_str(void *s)
{
	st_unalloc(s);
}

char	**grab_stack_str(char *p)
{
	char	*base;

	base = stack_block();
	if (p >= base)
		return ((char**)st_alloc((size_t)(p - base + 1)));
	return (NULL);
}

void	grab_stack_block(size_t len)
{
	(void)st_alloc(len);
}
