/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   geometry.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 15:05:44 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/03 15:22:53 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GEOMETRY_H
# define GEOMETRY_H
#include "quaternion.h"
#include "matrices.h"

// 2D, 3D, 4D Vectors
typedef struct s_vec2
{
	float	x,
	float	y;
}			t_vec2;

typedef struct s_vec3
{
	float	x,
	float	y,
	float	z;
}			t_vec3;



// 2D, 3D, 4D Points (alias for vectors)
typedef t_vec2 t_point2;
typedef t_vec3 t_point3;
typedef t_vec4 t_point4;

// Line (2 points)
typedef struct
{
	t_point3	a;
	t_point3	b;
}				t_line3;

typedef struct
{
	t_point2	a;
	t_point2	b;
}				t_line2;

// Ray (origin + direction)
typedef struct
{
	t_point3	origin;
	t_vec3		dir;
}				t_ray3;
typedef struct
{
	t_point2	origin;
	t_vec2		dir;
}				t_ray2;

// Plane (normal + distance)
typedef struct
{
	t_vec3	normal;
	float	d;
}			t_plane;

// Triangle
typedef struct s_triangle3
{
	t_point3	a;
	t_point3	b;
	t_point3	c;
}				t_triangle3;

typedef struct s_triangle2
{
	t_point2	a;
	t_point2	b;
	t_point2	c;
}				t_triangle2;

// Rectangle (axis-aligned, min/max)
typedef struct 
{
	t_point2	min,
	t_point2	max;
}				t_rect2;
typedef struct s_aabb3
{
	t_point3	min,
	t_point3	max;
}				t_aabb3;

// Oriented Bounding Box (center, axes, half-extents)
typedef struct
{
    t_point3	center;
    t_vec3		axis[3];     // local x, y, z axes (normalized)
    t_vec3		half_extents;
}				t_obb3;

// Sphere
typedef struct
{
	t_point3	center;
	float		radius;
}				t_sphere;

// Polygon (dynamic array)
typedef struct
{
    t_point3	*points;
    int			count;
} t_polygon3;

typedef struct {
    t_point2	*points;
    int			count;
} t_polygon2;

// Color (RGBA)
typedef struct
{
	uint32_t	r,
	uint32_t	g;
	uint32_t	b,
	uint32_t	a;
}				t_color;

// Transform (position, rotation, scale)
typedef struct
{
    t_vec3	position;
    t_quat	rotation;
    t_vec3	scale;
}			t_transform;

// Collision result structure
typedef struct
{
    int			hit;            // 1 if collision, 0 otherwise
    float		distance;     // Distance to collision
    t_point3	point;     // Collision/contact point
    t_vec3		normal;      // Surface normal at collision
    void		*object;       // Optional: pointer to collided object
}				t_collision;

typedef enum {
    SHAPE_NONE,
    SHAPE_SPHERE,
    SHAPE_AABB,
    SHAPE_OBB,
    SHAPE_TRIANGLE,
    SHAPE_PLANE,
    SHAPE_LINE,
    SHAPE_POLYGON2,
    SHAPE_POLYGON3,
} t_shape_type;

typedef struct
{
    t_shape_type type;
    union {
        t_sphere    sphere;
        t_aabb3     aabb;
        t_obb3      obb;
        t_triangle3 triangle3;
        t_triangle2 triangle2;
        t_plane     plane;
        t_line3     line3;
        t_line2     line2;
        t_polygon2  polygon2;
        t_polygon3  polygon3;
    };
} t_shape;

#endif