/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 23:39:35 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/26 23:49:14 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "geometry.h"
#include "matrices.h"
#include "ds.h"

bool	intersect_plane(t_plane plane, t_ray r, float *t)
{
	float	denom;

	denom = fvec3_dot(r.dir, plane.dir);
	if (fabs(denom) <= 1e-6)
		return (false);
	*t = fvec3_dot(fvec3_sub(plane.pos, r.pos), plane.dir) / denom;
	return (true);
}

float intersect_plane_generic(
    const float plane_point[3],
    const float plane_normal[3],
    const float ray_origin[3],
    const float ray_dir[3]
) {
    float	denom;
	float	diff[3];

	denom = dot3(ray_dir, plane_normal);
    if (fabsf(denom) <= 1e-6f)
        return (NAN);
    sub3(plane_point, ray_origin, diff);
    return (dot3(diff, plane_normal) / denom);
}

// Returns the parameter t at which the line (origin + t*dir) intersects the plane
// defined by (plane_point, plane_normal). Returns NAN if no intersection (parallel).
float intersect_line_plane(
    const float plane_point[3],
    const float plane_normal[3],
    const float line_origin[3],
    const float line_dir[3]
) {
    float denom = dot3(line_dir, plane_normal);
    if (fabsf(denom) <= 1e-6f)
        return NAN; // No intersection (parallel)
    float diff[3];
    sub3(plane_point, line_origin, diff);
    return dot3(diff, plane_normal) / denom;
}

