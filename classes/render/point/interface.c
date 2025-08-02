/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 04:58:32 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/30 05:36:44 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "point.h"

t_point_vtable	*singleton_vpoint(void)
{
	static t_point_vtable	vpoint = \
	{
		.destroy = point_destroy,
		.get_coordinate = point_get_coordinate,
		.get_position = point_get_position,
		.get_color = point_get_color,
		.set_coordinate = point_set_coordinate,
		.set_position = point_set_position,
		.set_color = point_set_color,
		.set_z_value = point_set_z_value,
		.translate = point_translate,
		.scale = point_scale,
		.clone = point_clone
	};

	return (&vpoint);
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

void	point_destroy(t_point *self)
{
	if (self)
		free(self);
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
	point->speed = 10.0;
	return (point);
}

t_point	*point_new_with_color(int x, int y, int z, uint32_t color)
{
	t_point	*point;

	point = point_new(x, y, z);
	if (!point)
		return (NULL);
	point->vtable->set_color(point, color);
	return (point);
}
