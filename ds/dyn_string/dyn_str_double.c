/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dyn_str_double.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:35:36 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/30 23:42:37 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dyn_string.h"

int	dyn_str_double(t_dyn_str *v)
{
	char	*temp;
	size_t	i;

	v->cap *= 2;
	if (v->cap == 0)
		v->cap = 64;
	temp = malloc(sizeof(int) * v->cap);
	if (!temp)
		return (0);
	i = -1;
	while (++i < v->len)
		temp[i] = v->buff[i];
	free(v->buff);
	v->buff = temp;
	return (1);
}
