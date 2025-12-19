/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dyn_str_push.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:35:53 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/19 02:24:31 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dyn_string.h"

/* internal implementation (restore original behavior) */
int	st_push(char el)
{
	t_dyn_str	*v;

	v = get_dyn_str(NULL);
	if (v->len + 1 >= v->cap || v->buff == 0)
		if (str_double())
			return (0);
	v->buff[v->len++] = el;
	v->buff[v->len] = 0;
	return (1);
}

int	str_push(char el)
{
	t_dyn_str	*v;

	v = get_dyn_str(NULL);
	if (!v)
		return (0);
	if (!v->buff || v->len + 1 >= v->cap)
		if (!str_double())
			return (0);
	v->buff[v->len++] = el;
	v->buff[v->len] = 0;
	return (1);
}

void	dyn_str_push(t_dyn_str *s, char el)
{
	ft_assert(s);
	if (s->len + 1 >= s->cap)
		if (!dyn_str_double(s))
			return (0);
	s->buff[s->len++] = el;
	s->buff[s->len] = 0;
	return (1);
}
