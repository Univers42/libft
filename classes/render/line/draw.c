/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:08:07 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/02 18:50:21 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line.h"
#include "ft_math.h"

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
	// Fix: pass second argument to get_coordinate
	self->start->vtable->get_coordinate(self->start, &start);
	self->end->vtable->get_coordinate(self->end, &end);
	dx = ft_abs(end.x - start.x);
	dy = ft_abs(end.y - start.y);
	sx = start.x < end.x ? 1 : -1;
	sy = start.y < end.y ? 1 : -1;
	err = dx - dy;
	x = start.x;
	y = start.y;
	total_steps = ft_sqrt(dx * dx + dy * dy);
	while (1)
	{
		t = (total_steps > 0) ? ft_sqrt((x - start.x) * (x - start.x)
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
	// Fix: pass second argument to get_coordinate
	self->start->vtable->get_coordinate(self->start, &start);
	self->end->vtable->get_coordinate(self->end, &end);
	dx = end.x - start.x;
	dy = end.y - start.y;
	steps = ft_abs(dx) > ft_abs(dy) ? ft_abs(dx) : ft_abs(dy);
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
