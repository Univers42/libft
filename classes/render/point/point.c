/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   point.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 00:14:31 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/28 00:57:37 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "point.h"

t_point_vtable *singleton_vpoint(void)
{
    static t_point_vtable vpoint = 
    {
        .get_coordinate = point_get_coordinate,
        .get_position = point_get_position,
        .get_color = point_get_color,
        .set_coordinate = point_set_coordinate,
        .set_position = point_set_position,
        .set_color = point_set_color,
        .set_z_value = point_set_z_value,
        .translate = point_translate,
        .scale = point_scale,
        .clone = point_clone,
        .destroy = point_destroy_method
    };
    return &vpoint;
}

t_point	*point_new(int x, int y, int z)
{
	t_point	*point;

	point = malloc(sizeof(t_point));
	if (!point)
		return (NULL);
	point->coordinate.x = x;
	point->coordinate.y = y;
	point->coordinate.u_ctx.s_v3.z = z;
	point->world_pos.x = (double)x;
	point->world_pos.y = (double)y;
	point->world_pos.z = (double)z;
	point->color.hex_color = 0xFFFFFFFF;
	point->color.s_rgba.r = 255;
	point->color.s_rgba.g = 255;
	point->color.s_rgba.b = 255;
	point->color.s_rgba.a = 255;
	point->vtable = singleton_vpoint();
	return (point);
}

t_point *point_new_with_color(int x, int y, int z, uint32_t color)
{
    t_point *point;

    point = point_new(x, y, z);
    if (!point)
        return (NULL);
    point->vtable->set_color(point, color);
    return (point);
}

void point_destroy(t_point *point)
{
    if (point)
        point->vtable->destroy(point);
}

t_vec2  point_get_coordinate(t_point *self)
{
    if (!self)
        return (t_vec2){0,0,{.s_v3 ={0}}};
    return (self->coordinate);
}

t_position  point_get_position(t_point *self)
{
    if (!self)
        return (t_position){0,0,0};
    return (self->world_pos);
}

t_color  point_get_color(t_point *self)
{
    if (!self)
        return (t_color){0, {0, 0, 0, 0}};
    return (self->color);
}

bool point_set_coordinate(t_point *self, int x, int y)
{
    if (!self)
        return (false);
    self->coordinate.x = x;
    self->coordinate.y = y;
    self->world_pos.x = (double)x;
    self->world_pos.y = (double)y;
    return (true);
}

bool point_set_position(t_point *self, double x, double y, double z)
{
    if (!self)
        return (false);
    self->world_pos.x = x;
    self->world_pos.y = y;
    self->world_pos.z = z;
    self->coordinate.x = (int)x;
    self->coordinate.y = (int)y;
    self->coordinate.u_ctx.s_v3.z = (int)z;
    return (true);
}

bool point_set_color(t_point *self, uint32_t hex_color)
{
    if (!self)
        return (false);
    self->color.hex_color = hex_color;
    hex_to_rgb(hex_color, &self->color.s_rgba.r,
        &self->color.s_rgba.g,
        &self->color.s_rgba.b);
    self->color.s_rgba.a = (hex_color >> 24) & 0xFF;
    return (true);
}

bool point_set_z_value(t_point *self, int z)
{
    if (!self)
        return (false);
    self->coordinate.u_ctx.s_v3.z = z;
    self->world_pos.z = (double)z;
    return (true);
}

bool	point_translate(t_point *self, int dx, int dy)
{
	if (!self)
		return (false);
	self->coordinate.x += dx;
	self->coordinate.y += dy;
	self->world_pos.x += (double)dx;
	self->world_pos.y += (double)dy;
	return (true);
}

bool	point_scale(t_point *self, double scale_x, double scale_y)
{
	if (!self)
		return (false);
	self->world_pos.x *= scale_x;
	self->world_pos.y *= scale_y;
	self->coordinate.x = (int)(self->world_pos.x);
	self->coordinate.y = (int)(self->world_pos.y);
	return (true);
}

t_point	*point_clone(t_point *self)
{
	t_point	*clone;

	if (!self)
		return (NULL);
	clone = point_new_with_color(self->coordinate.x, self->coordinate.y,
			self->coordinate.u_ctx.s_v3.z, self->color.hex_color);
	if (!clone)
		return (NULL);
	clone->world_pos = self->world_pos;
	return (clone);
}

void	point_destroy_method(t_point *self)
{
	if (self)
		free(self);
}

/* Utility functions */
uint32_t	rgb_to_hex(uint8_t r, uint8_t g, uint8_t b)
{
	return ((uint32_t)0xFF << 24) | ((uint32_t)r << 16)
		| ((uint32_t)g << 8) | (uint32_t)b;
}

void	hex_to_rgb(uint32_t hex, uint8_t *r, uint8_t *g, uint8_t *b)
{
	if (!r || !g || !b)
		return ;
	*r = (hex >> 16) & 0xFF;
	*g = (hex >> 8) & 0xFF;
	*b = hex & 0xFF;
}

double	point_distance(t_point *p1, t_point *p2)
{
	double	dx;
	double	dy;
	double	dz;

	if (!p1 || !p2)
		return (-1.0);
	dx = p1->world_pos.x - p2->world_pos.x;
	dy = p1->world_pos.y - p2->world_pos.y;
	dz = p1->world_pos.z - p2->world_pos.z;
	return (sqrt(dx * dx + dy * dy + dz * dz));
}

bool	point_equals(t_point *p1, t_point *p2)
{
	if (!p1 || !p2)
		return (false);
	return (p1->coordinate.x == p2->coordinate.x
		&& p1->coordinate.y == p2->coordinate.y
		&& p1->coordinate.u_ctx.s_v3.z == p2->coordinate.u_ctx.s_v3.z);
}