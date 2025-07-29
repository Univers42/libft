/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 01:10:21 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/28 11:01:50 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line.h"
#include <math.h>
#include <stdlib.h>

/* Static vtable instance */
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

/* Vtable method implementations */
t_point	*line_get_start(t_line *self)
{
	if (!self)
		return (NULL);
	return (self->start);
}

t_point	*line_get_end(t_line *self)
{
	if (!self)
		return (NULL);
	return (self->end);
}

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

double	line_get_length(t_line *self)
{
	if (!self || !self->start || !self->end)
		return (-1.0);
	return (point_distance(self->start, self->end));
}

double	line_get_slope(t_line *self)
{
	t_vec2	start_coord;
	t_vec2	end_coord;
	int		dx;
	int		dy;

	if (!self || !self->start || !self->end)
		return (0.0);
	start_coord = self->start->vtable->get_coordinate(self->start);
	end_coord = self->end->vtable->get_coordinate(self->end);
	dx = end_coord.x - start_coord.x;
	dy = end_coord.y - start_coord.y;
	if (dx == 0)
		return (INFINITY);
	return ((double)dy / (double)dx);
}

bool	line_is_vertical(t_line *self)
{
	t_vec2	start_coord;
	t_vec2	end_coord;

	if (!self || !self->start || !self->end)
		return (false);
	start_coord = self->start->vtable->get_coordinate(self->start);
	end_coord = self->end->vtable->get_coordinate(self->end);
	return (start_coord.x == end_coord.x);
}

bool	line_is_horizontal(t_line *self)
{
	t_vec2	start_coord;
	t_vec2	end_coord;

	if (!self || !self->start || !self->end)
		return (false);
	start_coord = self->start->vtable->get_coordinate(self->start);
	end_coord = self->end->vtable->get_coordinate(self->end);
	return (start_coord.y == end_coord.y);
}

uint32_t	line_interpolate_color(t_line *self, double t)
{
	t_color	start_color;
	t_color	end_color;

	if (!self || !self->start || !self->end || t < 0.0 || t > 1.0)
		return (0);
	start_color = self->start->vtable->get_color(self->start);
	end_color = self->end->vtable->get_color(self->end);
	return (color_lerp(start_color.hex_color, end_color.hex_color, t));
}

t_point	*line_interpolate_point(t_line *self, double t)
{
	t_position	start_pos;
	t_position	end_pos;
	double		x;
	double		y;
	double		z;

	if (!self || !self->start || !self->end || t < 0.0 || t > 1.0)
		return (NULL);
	start_pos = self->start->vtable->get_position(self->start);
	end_pos = self->end->vtable->get_position(self->end);
	x = start_pos.x + t * (end_pos.x - start_pos.x);
	y = start_pos.y + t * (end_pos.y - start_pos.y);
	z = start_pos.z + t * (end_pos.z - start_pos.z);
	return (point_new((int)x, (int)y, (int)z));
}

void	line_draw_bresenham(t_line *self, t_pixel_callback callback, void *data)
{
	t_vec2		start;
	t_vec2		end;
	int			dx;
	int			dy;
	int			sx;
	int			sy;
	int			err;
	int			e2;
	int			x;
	int			y;
	double		t;
	double		total_steps;
	uint32_t	color;

	if (!self || !self->start || !self->end || !callback)
		return ;
	start = self->start->vtable->get_coordinate(self->start);
	end = self->end->vtable->get_coordinate(self->end);
	dx = abs(end.x - start.x);
	dy = abs(end.y - start.y);
	sx = start.x < end.x ? 1 : -1;
	sy = start.y < end.y ? 1 : -1;
	err = dx - dy;
	x = start.x;
	y = start.y;
	total_steps = sqrt(dx * dx + dy * dy);
	while (1)
	{
		t = (total_steps > 0) ? sqrt((x - start.x) * (x - start.x)
				+ (y - start.y) * (y - start.y)) / total_steps : 0.0;
		color = self->vtable->interpolate_color(self, t);
		callback(x, y, color, data);
		if (x == end.x && y == end.y)
			break ;
		e2 = 2 * err;
		if (e2 > -dy)
		{
			err -= dy;
			x += sx;
		}
		if (e2 < dx)
		{
			err += dx;
			y += sy;
		}
	}
}

void	line_draw_dda(t_line *self, t_pixel_callback callback, void *data)
{
	t_vec2		start;
	t_vec2		end;
	int			dx;
	int			dy;
	int			steps;
	double		x_inc;
	double		y_inc;
	double		x;
	double		y;
	int			i;
	double		t;
	uint32_t	color;

	if (!self || !self->start || !self->end || !callback)
		return ;
	start = self->start->vtable->get_coordinate(self->start);
	end = self->end->vtable->get_coordinate(self->end);
	dx = end.x - start.x;
	dy = end.y - start.y;
	steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
	x_inc = (double)dx / (double)steps;
	y_inc = (double)dy / (double)steps;
	x = (double)start.x;
	y = (double)start.y;
	i = 0;
	while (i <= steps)
	{
		t = (steps > 0) ? (double)i / (double)steps : 0.0;
		color = self->vtable->interpolate_color(self, t);
		callback((int)x, (int)y, color, data);
		x += x_inc;
		y += y_inc;
		i++;
	}
}

bool	line_translate(t_line *self, int dx, int dy)
{
	if (!self || !self->start || !self->end)
		return (false);
	self->start->vtable->translate(self->start, dx, dy);
	self->end->vtable->translate(self->end, dx, dy);
	return (true);
}

bool	line_scale(t_line *self, double scale_x, double scale_y)
{
	if (!self || !self->start || !self->end)
		return (false);
	self->start->vtable->scale(self->start, scale_x, scale_y);
	self->end->vtable->scale(self->end, scale_x, scale_y);
	return (true);
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

/* Utility functions */
uint32_t	color_lerp(uint32_t color1, uint32_t color2, double t)
{
	uint8_t	r1;
	uint8_t	g1;
	uint8_t	b1;
	uint8_t	a1;
	uint8_t	r2;
	uint8_t	g2;
	uint8_t	b2;
	uint8_t	a2;
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;
	uint8_t	a;

	if (t < 0.0)
		t = 0.0;
	if (t > 1.0)
		t = 1.0;
	a1 = (color1 >> 24) & 0xFF;
	r1 = (color1 >> 16) & 0xFF;
	g1 = (color1 >> 8) & 0xFF;
	b1 = color1 & 0xFF;
	a2 = (color2 >> 24) & 0xFF;
	r2 = (color2 >> 16) & 0xFF;
	g2 = (color2 >> 8) & 0xFF;
	b2 = color2 & 0xFF;
	a = (uint8_t)(a1 + t * (a2 - a1));
	r = (uint8_t)(r1 + t * (r2 - r1));
	g = (uint8_t)(g1 + t * (g2 - g1));
	b = (uint8_t)(b1 + t * (b2 - b1));
	return ((uint32_t)a << 24) | ((uint32_t)r << 16)
		| ((uint32_t)g << 8) | (uint32_t)b;
}

bool	line_equals(t_line *line1, t_line *line2)
{
	if (!line1 || !line2)
		return (false);
	return (point_equals(line1->start, line2->start)
		&& point_equals(line1->end, line2->end));
}

double	line_distance_to_point(t_line *line, t_point *point)
{
	t_position	start;
	t_position	end;
	t_position	p;
	double		line_len_sq;
	double		t;
	double		projection_x;
	double		projection_y;
	double		dx;
	double		dy;

	if (!line || !point || !line->start || !line->end)
		return (-1.0);
	start = line->start->vtable->get_position(line->start);
	end = line->end->vtable->get_position(line->end);
	p = point->vtable->get_position(point);
	line_len_sq = (end.x - start.x) * (end.x - start.x)
		+ (end.y - start.y) * (end.y - start.y);
	if (line_len_sq == 0.0)
		return (sqrt((p.x - start.x) * (p.x - start.x)
				+ (p.y - start.y) * (p.y - start.y)));
	t = ((p.x - start.x) * (end.x - start.x)
			+ (p.y - start.y) * (end.y - start.y)) / line_len_sq;
	if (t < 0.0)
		t = 0.0;
	if (t > 1.0)
		t = 1.0;
	projection_x = start.x + t * (end.x - start.x);
	projection_y = start.y + t * (end.y - start.y);
	dx = p.x - projection_x;
	dy = p.y - projection_y;
	return (sqrt(dx * dx + dy * dy));
}