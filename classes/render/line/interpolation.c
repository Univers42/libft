/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpolation.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:08:59 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/01 10:52:24 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line.h"

t_point	*line_interpolate_point(t_line *self, double t)
{
	t_position	start_pos;
	t_position	end_pos;
	double		x;
	double		y;
	double		z;

	if (!self || !self->start || !self->end || t < 0.0 || t > 1.0)
		return (NULL);
	self->start->vtable->get_position(self->start, &start_pos);
	self->end->vtable->get_position(self->end, &end_pos);
	x = start_pos.x + t * (end_pos.x - start_pos.x);
	y = start_pos.y + t * (end_pos.y - start_pos.y);
	z = start_pos.z + t * (end_pos.z - start_pos.z);
	return (point_new((int)x, (int)y, (int)z));
}
