/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 04:57:56 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/30 05:36:43 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "point.h"

void	point_get_coordinate(t_point *self, t_vec2 *out)
{
	if (!self || !out)
		return ;
	*out = self->coordinate;
}

void	point_get_position(t_point *self, t_position *out)
{
	if (!self || !out)
		return ;
	*out = self->world_pos;
}

void	point_get_color(t_point *self, t_color *out)
{
	if (!self || !out)
		return ;
	*out = self->color;
}
