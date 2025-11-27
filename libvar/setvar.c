/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setvar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 16:09:40 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/27 17:03:04 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "var.h"

static t_var	*build_and_set(t_meta *key, t_meta *val, int flags)
{
	char	*name_eq;
	char	*p;
	t_var	*vp;

	name_eq = ckmalloc(key->len + val->len + 2);
	p = ft_memcpy(name_eq, key->name, key->len);
	if (val)
	{
		*p++ = '=';
		p = ft_memcpy(p, val, val->len);
	}
	*p = '\0';
	vp = setvareq(name_eq, flags | VNO_SAVE);
	return (vp);
}

t_var	*setvar(const char *name, const char *val, int flags)
{
	t_meta	key;
	t_meta	value;
	char	*p;
	char	*q;
	t_var	*vp;

	key.name = name;
	value.name = val;
	q = end_of_name(key.name);
	p = ft_strchrnul(q, '=');
	key.len = p - key.name;
	if (!key.len || p != q)
		return (NULL);
	value.len = 0;
	if (!value.name)
		flags |= VUNSET;
	else
		value.len = ft_strlen(value.name);
	intoff();
	vp = build_and_set(name, val, flags);
	inton();
	return (vp);
}
