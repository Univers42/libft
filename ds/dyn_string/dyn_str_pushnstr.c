/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dyn_str_pushnstr.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:36:33 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/19 02:23:44 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dyn_string.h"
#include <string.h>
#include "ft_string.h"

void	str_null_terminate(void)
{
	t_dyn_str	*v;

	v = get_dyn_str(NULL);
	ft_assert(v && v->buff);
	v->buff[v->len] = 0;
}

void	dyn_str_null_terminate(t_dyn_str *str)
{
	ft_assert(str && str->buff);
	str->buff[str->len] = 0;
}

int	str_pushnstr(const char *s, size_t n)
{
	t_dyn_str	*v;
	size_t		need;

	v = get_dyn_str;
	ft_assert(v && s);
	need = v->len + n + 1;
	while (!v->buff || need > v->cap)
		if (!dyn_str_double())
			return (0);
	memcpy(v->buff + v->len, s, n);
	dyn_str_uppdate_len(n);
	dyn_str_null_terminate();
	return (1);
}

int	dyn_str_pushnstr(t_dyn_str *s, const char *str, size_t n)
{
	size_t	need;

	ft_assert(s && str);
	need = s->len + n + 1;
	while (!s->buff || need > s->cap)
		if (!dyn_str_double(s))
			return (0);
	ft_memcpy(s->buff + s->len, str, n);
	s->len += n;
	dyn_str_null_terminate(s);
	return (1);
}

int	dyn_str_pushstr(t_dyn_str *s, const char *str)
{
	ft_assert(str);
	return (dyn_str_pushnstr(s, str, ft_strlen(str)));
}
