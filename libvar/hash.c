/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 16:09:58 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/28 17:46:47 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private_var.h"
#include "var.h"

char		*lookup_var(const char *name)
{
	t_var	*v;

	v = *find_var(name);
	if (v && !(v->flags & VUNSET))
}

intmax_t	lookup_var_int(const char *name)
{
	char	*value;

	value = lookupvar(name);
	if (value == NULL)
		return (ft_atomax("", 0));
	return (ft_atomax(value, 0));
}

t_var	**var_hash(const char *p)
{
	unsigned int	hashval;
	t_var_state		*state;

	state = get_var_state();
	hashval = ((unsigned char)*p) << 4;
	while (*p && *p != '=')
	{
		hashval += (unsigned char)*p;
		p++;
	}
	return (&state->vartab[hashval % VTABSIZE]);
}
