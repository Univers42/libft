/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:07:06 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/30 16:09:26 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line.h"

bool	line_set_start(t_line *self, t_point *start)
{
	if (!self || !start)
		return (false);
	if (self->owns_points && self->start)
		point_destroy(self->start);
	self->start = start;
	return (true);
}

bool	line_set_end(t_line *self, t_point *end)
{
	if (!self || !end)
		return (false);
	if (self->owns_points && self->end)
		point_destroy(self->end);
	self->end = end;
	return (true);
}

bool	line_set_points(t_line *self, t_point *start, t_point *end)
{
	if (!self || !start || !end)
		return (false);
	if (self->owns_points)
	{
		if (self->start)
			point_destroy(self->start);
		if (self->end)
			point_destroy(self->end);
	}
	self->start = start;
	self->end = end;
	return (true);
}