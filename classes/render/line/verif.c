/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verif.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:07:35 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/02 16:00:14 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line.h"

bool	line_is_vertical(t_line *self)
{
	t_vec2	start_coord;
	t_vec2	end_coord;

	if (!self || !self->start || !self->end)
		return (false);
	self->start->vtable->get_coordinate(self->start, &start_coord);
	self->end->vtable->get_coordinate(self->end, &end_coord);
	return (start_coord.x == end_coord.x);
}

bool	line_is_horizontal(t_line *self)
{
	t_vec2	start_coord;
	t_vec2	end_coord;

	if (!self || !self->start || !self->end)
		return (false);
	self->start->vtable->get_coordinate(self->start, &start_coord);
	self->end->vtable->get_coordinate(self->end, &end_coord);
	return (start_coord.y == end_coord.y);
}


bool	line_equals(t_line *line1, t_line *line2)
{
	if (!line1 || !line2)
		return (false);
	return (point_equals(line1->start, line2->start)
		&& point_equals(line1->end, line2->end));
}

