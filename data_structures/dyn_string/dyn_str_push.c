/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dyn_str_push.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:35:53 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/26 01:37:52 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dyn_string.h"

int	dyn_st_push(t_dyn_str *v, char el)
{
	if (v->len + 1 >= v->cap || v->buff == 0)
		if (dyn_str_double(v))
			return (0);
	v->buff[v->len++] = el;
	v->buff[v->len] = 0;
	return (1);
}
