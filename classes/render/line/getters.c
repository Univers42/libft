/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:04:20 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/02 18:50:51 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line.h"
#include <math.h> // Add this include for INFINITY

/* Vtable method implementations */
t_point *line_get_start(t_line *self)
{
	if (!self)
		return (NULL);
	return (self->start);
}

t_point *line_get_end(t_line *self)
{
	if (!self)
		return (NULL);
	return (self->end);
}

double line_get_length(t_line *self)
{
	if (!self || !self->start || !self->end)
		return (-1.0);
	return (euclidean_dist(self->start, self->end));
}

double line_get_slope(t_line *self)
{
	t_vec2 start_coord;
	t_vec2 end_coord;
	int dx;
	int dy;

	if (!self || !self->start || !self->end)
		return (0.0);
	self->start->vtable->get_coordinate(self->start, &start_coord);
	self->end->vtable->get_coordinate(self->end, &end_coord);
	dx = end_coord.x - start_coord.x;
	dy = end_coord.y - start_coord.y;
	if (dx == 0)
		return (INFINITY);
	return ((double)dy / (double)dx);
}