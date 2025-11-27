/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_hash.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 16:07:09 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/27 16:42:35 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "var.h"

struct s_var	**var_hash(const char *p)
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
