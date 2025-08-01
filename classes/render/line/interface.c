/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:05:50 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/01 10:52:26 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line.h"

static t_line_vtable	g_line_vtable = {
	.get_start = line_get_start,
	.get_end = line_get_end,
	.set_start = line_set_start,
	.set_end = line_set_end,
	.set_points = line_set_points,
	.get_length = line_get_length,
	.get_slope = line_get_slope,
	.is_vertical = line_is_vertical,
	.is_horizontal = line_is_horizontal,
	.interpolate_color = line_interpolate_color,
	.interpolate_point = line_interpolate_point,
	.draw_bresenham = line_draw_bresenham,
	.draw_dda = line_draw_dda,
	.translate = line_translate,
	.scale = line_scale,
	.clone = line_clone,
	.destroy = line_destroy_method
};

/* Constructor - references existing points */
t_line	*line_new(t_point *start, t_point *end)
{
	t_line	*line;

	if (!start || !end)
		return (NULL);
	line = malloc(sizeof(t_line));
	if (!line)
		return (NULL);
	line->start = start;
	line->end = end;
	line->owns_points = false;
	line->vtable = &g_line_vtable;
	return (line);
}

/* Constructor - creates colored points */
t_line	*line_new_with_colors(int x1, int y1, int z1, uint32_t color1,
		int x2, int y2, int z2, uint32_t color2)
{
	t_point	*start;
	t_point	*end;
	t_line	*line;

	start = point_new_with_color(x1, y1, z1, color1);
	end = point_new_with_color(x2, y2, z2, color2);
	if (!start || !end)
	{
		if (start)
			point_destroy(start);
		if (end)
			point_destroy(end);
		return (NULL);
	}
	line = line_new_owned(start, end);
	return (line);
}

/* Destructor */
void	line_destroy(t_line *line)
{
	if (line)
		line->vtable->destroy(line);
}

void	line_destroy_method(t_line *self)
{
	if (!self)
		return ;
	if (self->owns_points)
	{
		if (self->start)
			point_destroy(self->start);
		if (self->end)
			point_destroy(self->end);
	}
	free(self);
}


t_line	*line_clone(t_line *self)
{
	t_point	*start_clone;
	t_point	*end_clone;

	if (!self || !self->start || !self->end)
		return (NULL);
	start_clone = self->start->vtable->clone(self->start);
	end_clone = self->end->vtable->clone(self->end);
	if (!start_clone || !end_clone)
	{
		if (start_clone)
			point_destroy(start_clone);
		if (end_clone)
			point_destroy(end_clone);
		return (NULL);
	}
	return (line_new_owned(start_clone, end_clone));
}

/* Constructor - creates points from coordinates */
t_line	*line_new_coordinates(int x1, int y1, int z1, int x2, int y2, int z2)
{
	t_point	*start;
	t_point	*end;
	t_line	*line;

	start = point_new(x1, y1, z1);
	end = point_new(x2, y2, z2);
	if (!start || !end)
	{
		if (start)
			point_destroy(start);
		if (end)
			point_destroy(end);
		return (NULL);
	}
	line = line_new_owned(start, end);
	return (line);
}

/* Constructor - takes ownership of points */
t_line	*line_new_owned(t_point *start, t_point *end)
{
	t_line	*line;

	line = line_new(start, end);
	if (!line)
		return (NULL);
	line->owns_points = true;
	return (line);
}

