/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 04:59:04 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/30 05:36:44 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "point.h"

int	point_set_coordinate(t_point *self, int x, int y)
{
	if (!self)
		return (0);
	self->coordinate.x = x;
	self->coordinate.y = y;
	self->world_pos.x = (double)x;
	self->world_pos.y = (double)y;
	return (1);
}

int	point_set_position(t_point *self, double x, double y, double z)
{
	if (!self)
		return (0);
	self->world_pos.x = x;
	self->world_pos.y = y;
	self->world_pos.z = z;
	self->coordinate.x = (int)x;
	self->coordinate.y = (int)y;
	self->coordinate.u_ctx.s_v3.z = (int)z;
	return (1);
}

int	point_set_color(t_point *self, uint32_t hex_color)
{
	if (!self)
		return (0);
	self->color.hex_color = hex_color;
	hex_to_rgb(hex_color, &self->color.s_rgba.r,
		&self->color.s_rgba.g,
		&self->color.s_rgba.b);
	self->color.s_rgba.a = (hex_color >> 24) & 0xFF;
	return (1);
}

int	point_set_z_value(t_point *self, int z)
{
	if (!self)
		return (0);
	self->coordinate.u_ctx.s_v3.z = z;
	self->world_pos.z = (double)z;
	return (1);
}

int	point_scale(t_point *self, double scale_x, double scale_y)
{
	if (!self)
		return (0);
	self->world_pos.x *= scale_x;
	self->world_pos.y *= scale_y;
	self->coordinate.x = (int)(self->world_pos.x);
	self->coordinate.y = (int)(self->world_pos.y);
	return (1);
}
