/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dyn_str_pop.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:36:29 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/09 01:47:07 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dyn_string.h"

char	dyn_str_pop(t_dyn_str *v)
{
	char	ret;

	ft_assert(v->len > 0);
	ret = v->buff[--v->len];
	v->buff[v->len] = 0;
	return (ret);
}