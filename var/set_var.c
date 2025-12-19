/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 03:53:59 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/19 03:54:53 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "private_var.h"
#include "var.h"

#define INTMAX_BUFSIZE 25

static t_var *build_and_set(t_dyn_str *key, t_dyn_str *val, int flags);

// Main public API
intmax_t set_varint(char *name, intmax_t val, int flags)
{
	char buf[INTMAX_BUFSIZE];

	fmtstr(buf, INTMAX_BUFSIZE, "%" PRIdMAX, val);
	set_var(name, buf, flags);
	return (val);
}

t_var *set_var(char *name, char *val, int flags)
{
	t_dyn_str	key;
	t_dyn_str	value;
	char	*p;
	char	*q;
	t_var	*vp;

	key.buff = (char *)name;
	value.buff = (char *)val;
	q = end_of_name(key.buff);
	p = ft_strchrnul(q, '=');
	key.len = p - key.buff;
	if (!key.len || p != q)
		return (NULL);
	value.len = 0;
	if (!value.buff)
		flags |= VUNSET;
	else
		value.len = ft_strlen(value.buff);
	intoff();
	vp = build_and_set(&key, &value, flags);
	inton();
	return (vp);
}

// Private helper

static t_var	*build_and_set(t_dyn_str *key, t_dyn_str *val, int flags)
{
	char *name_eq;
	char *p;
	t_var *vp;

	if (val->buff)
		name_eq = xmalloc((size_t)key->len + val->len + 2);
	else
		name_eq = xmalloc((size_t)key->len + 1);
	ft_memcpy(name_eq, key->buff, key->len);
	p = name_eq + key->len;
	if (val->buff)
	{
		*p++ = '=';
		if (val->len)
		{
			ft_memcpy(p, val->buff, val->len);
			p += val->len;
		}
	}
	*p = '\0';
	vp = set_vareq(name_eq, flags | VNO_SAVE);
	return (vp);
}
