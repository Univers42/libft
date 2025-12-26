/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collide_shapes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 21:33:18 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/26 23:39:40 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "geometry.h"
#include "matrices.h"
#include "ds.h"

static inline void collide_shape_sphere(t_sphere sphere,
						t_ray_isector isector,
						t_collision *ret)
{
	ret->collided = intersect_sphere(sphere, isector.ray, &ret->t);
}

static inline void	collide_shape_cylinder(t_cylinder cylinder,
						t_ray_isector isector, t_collision *ret)
{
	ret->collided = interset_cylinder(isector.ray, cylinder, &ret->t, &ret->disk_collision);
}

t_collision	collide_shape()
{
	t_collision	ret;

	ret = (t_collision){.shape = shape};
	if (shape.type == OBJ_TRIANGLE)
		collide_shape_triangle();
	else if (shape.type == OBJ_SPHERE)
		collide_shape_sphere(*(t_sphere *)shape.ptr, isector, &ret);
	else if (shape.type == OBJ_CYLINDER)
		collide_shape_cylinder(*(t_cylinder *)shape.ptr, isector, &ret);
	else if (shape.type == OBJ_PLANE)
		ret.collided = intersect_plane(*(t_plane *)shape.ptr, isector.ray, &ret.t);
	else
		ft_assert("Unreachable" != 0);
	return (ret);
}