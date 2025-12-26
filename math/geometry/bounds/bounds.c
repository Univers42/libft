/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bounds.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 23:52:30 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/27 00:16:24 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "geometry.h"
#include "ft_stddef.h"

typedef struct s_bounds3f
{
	t_fvec3	min;
	t_fvec3	max;
} t_bounds3f;

inline static t_bounds3f	bounds_degenerate(void)
{
	return ((t_bounds3f){{NAN, NAN, NAN}, {NAN, NAN, NAN}});
}

t_bounds3f	bounds_extend_pt(t_bounds3f bounds, t_fvec3 pt)
{
	bounds.min.x = fmin(bounds.min.x, pt.x);
	bounds.min.y = fmin(bounds.min.y, pt.y);
	bounds.min.z = fmin(bounds.min.z, pt.z);
	bounds.max.x = fmax(bounds.max.x, pt.x);
	bounds.max.y = fmax(bounds.max.y, pt.y);
	bounds.max.z = fmax(bounds.max.z, pt.z);
	return (bounds);
}

t_bounds3f	bounds_extend_bounds(t_bounds3f b1, t_bounds3f b2)
{
	b1 = bounds_extend_pt(b1, b2.min);
	b1 = bounds_extend_pt(b1, b2.max);
}

t_fvec3	bounds_centroid(t_bounds3f bounds)
{
	return (fvec3_add(bounds.min, fvec3_scale(fvec3_sub(bounds.max, bounds.min), 0.5)));
}


inline static float box_surface_area_arr(const float diag[3])
{
    return 2.0f * (diag[X] * diag[Y] + diag[X] * diag[Z] + diag[Y] * diag[Z]);
}

float bounds_surface_area(t_bounds3f bounds)
{
    t_fvec3 diag;

	diag= fvec3_sub(bounds.max, bounds.min);
    return (box_surface_area(diag));
}

uint8_t	bounds_max_dim(t_bounds3f bounds)
{
	t_fvec3	diag;

	diag = fvec3_sub(bounds.max, bounds.min);
	if (diag.x >= diag.y && diag.x >= diag.z)
		return (0);
	if (diag.y >= diag.x && diag.y >= diag.z)
		return (1);
	if (diag.z >= diag.x && diag.z >= diag.y)
		return (2);
	return (2);
}

// Returns the normalized position of value in [min, max], clamped to [0, 1]
inline static float normalize_to_unit(float value, float min, float max)
{
	if (max == min)
		return (0.0f);
	return ((value - min) / (max - min));	
}

t_fvec3	bounds_offset(t_bounds3f bounds, t_fvec3 p)
{
    return (t_fvec3){
        .x = fmaxf(normalize_to_unit(p.x, bounds.min.x, bounds.max.x), 0.0f),
        .y = fmaxf(normalize_to_unit(p.y, bounds.min.y, bounds.max.y), 0.0f),
        .z = fmaxf(normalize_to_unit(p.z, bounds.min.z, bounds.max.z), 0.0f),
    };
}

bool	bounds_check_enclosed(t_bounds3f big, t_bounds3f small)
{
	if (big.min.x > small.min.x || big.min.y > small.min.y
		|| big.min.z > small.min.z || big.max.x < small.max.x
		|| big.max.y < small.max.y || big.max.z < small.max.z)
		return (false);
	return (true);
}

bool	intersect_bounds(t_bounds3f bounds,
			t_ray r,
			float t_max,
			float *hitt0)
{
	t_fvec2	ts;
	float	inv_ray_dir;
	float	t_near;
	float	t_far;
	int		i;

	ts.x = 0;
	ts.y = t_max;
	i = -1;
	while (++i < 3)
	{
		inv_ray_dir = 1 / fvec3_idx(r.dir, i);
		t_near = (fvec3_idx(bounds.min, i) - fvec3_idx(r.pos, i)) * inv_ray_dir;
		t_far = (fvec3_idx(bounds.max, i) - fvec3_idx(r.pos, i)) * inv_ray_dir;
		sort2f(&t_near, &t_far);
		t_far *= 1 + 2 * f32_gamma(3);
		ts.x = fmax(t_near, ts.x);
		ts.y = fmin(t_far, ts.y);
		if (ts.x > ts.y)
			return (false);
	}
	if (hitt0)
		*hitt0 = ts.x;
	return (true);
}
