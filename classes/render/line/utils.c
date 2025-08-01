/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:11:30 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/30 16:12:22 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line.h"

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
	line->start->vtable->get_position(line->start, &start);
	line->end->vtable->get_position(line->end, &end);
	point->vtable->get_position(point, &p);
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
