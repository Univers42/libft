/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   geometry.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 15:05:44 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/26 23:51:15 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GEOMETRY_H
# define GEOMETRY_H

# include <stdint.h>
# include <stdbool.h>
# include "pixel.h"
# include <math.h>
# include "matrices.h"
# include "ft_stdlib.h"
# include "ds.h"

/* Provide aliases expected by point.h (reuse pixel.h types) */
typedef t_point2							t_vec2;
typedef t_fpoint3							t_pos3;
typedef t_point3							t_pos;
typedef struct s_sphere_data				t_sphere_data;
/* Callback for transformations */
typedef void								(*t_transform_callback)\
											(t_sphere_data *data);

# ifndef M_PI
#  define M_PI 3.14159265358979323846
# endif

# define X 0
# define Y 1
# define Z 2


/**
 * good structure start point for ray_casting and ray tracing
 * There is enough for representing a ray in 3D space
 * Ray/geometry intersection tests
 * Ray tracing, picking, and basic physics
 */
typedef struct s_ray
{
	t_fvec3	dir;	//direction
	t_fvec3	pos;	// origin
	t_fvec3	inv_dir;	//precompute for faster AABB intersection
	// used for limiting the valid segment of the ray (for intersection intervals)
	float	min;
	float	max;
}	t_ray;

/* Spherical transformation data */
struct s_sphere_data
{
	t_fpoint2	*points;
	int			len;
	float		radius;
	t_fpoint3	limits;
	t_fpoint3	center;
	float		scale;
	bool		initialized;
};

typedef enum e_obj
{
	OBJ_SPHERE,
	OBJ_PLACE,
	OBJ_CYLINDER,
	OBJ_TRIANGLE
}	t_obj;

typedef struct s_triangle
{
	int	mesh_idx;
	int	triangle_idx;
}	t_triangle;

typedef struct s_sphere
{
	t_fvec3	p;
	float	r;
	int		spectrun_idx;
}t_sphere;

typedef struct s_cylinder
{
	t_fvec3	b;
	t_fvec3	dir;
	float	radius;
	float	height;
	int		spectrum_idx;
}	t_cylinder;

typedef struct s_ray_isector
{
	t_ray	ray;
	float	t_min;
	float	t_max;
	void	*ignore_shape;
	t_fvec3	prev_norm;
}	t_ray_isector;

typedef struct s_mesh
{
	t_vec_fvec3	vertices;
	t_vec		vertex_idxs;
	int			spectrum_idx;
}	t_mesh;

typedef struct s_plane
{
	t_fvec3	pos;
	t_fvec3	dir;
	int		spectrum_idx;	
}	t_plane;

typedef struct s_shape
{
		t_obj	type;
		void	*ptr;
}	t_shape;

typedef struct s_collision
{
	float	t;
	bool	collided;
	bool	disk_collision;
	float	u;
	float	v;
	t_shape	shape;
}	t_collision;

typedef struct s_triangle_collision
{
	float	t;
	float	u;
	float	v;
	t_shape	shape;
}	t_triangle_collision;


typedef struct s_triangle_isector
{
	float	inv_det;
	float	det;
	t_fvec3	edge1;
	t_fvec3	edge2;
	t_fvec3	ray_cross_e2;
	t_fvec3	s_cross_e1;
	t_fvec3	s;	
}	t_triangle_isector;

typedef struct s_isect_cylinder
{
	t_fvec3	b;
	t_fvec3	a;
	t_fvec3	n;
	t_fvec3	nxa;
	float	nxa_len_sq;
	t_fvec3	bxa;
	float	rhs;
}	t_isect_cylinder;

typedef struct s_triangle_pts
{
	t_fvec3	a;
	t_fvec3	b;
	t_fvec3	c;
}	t_triangle_pts;

/* Line types */
typedef struct s_line3
{
	t_point3	a;
	t_point3	b;
}	t_line3;

typedef struct s_line2
{
	t_point2	a;
	t_point2	b;
}	t_line2;

typedef struct s_rect2
{
	t_point2	min;
	t_point2	max;
}	t_rect2;

bool	intersect_plane(t_plane plane, t_ray r, float *t);
float intersect_plane_generic(
    const float plane_point[3],
    const float plane_normal[3],
    const float ray_origin[3],
    const float ray_dir[3]
);

float intersect_line_plane(
    const float plane_point[3],
    const float plane_normal[3],
    const float line_origin[3],
    const float line_dir[3]
);

#endif
