/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   geometry.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 15:05:44 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/06 20:38:40 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GEOMETRY_H
# define GEOMETRY_H
#include "quaternion.h"
#include "matrices.h"
#include "pixel.h"
#include <stdint.h>
#include <stdbool.h>

// Forward declaration for t_parser (from map.h)
typedef struct s_parser t_parser;

#ifndef M_PI
# define M_PI 3.14159265358979323846
#endif

// Axis indices
#define X 0
#define Y 1
#define Z 2



// 2D, 3D, 4D Vectors
typedef struct s_vec2
{
	float	x;
	float	y;
}			t_vec2;

typedef struct s_vec3
{
	float	x;
	float	y;
	float	z;
}			t_vec3;

// Enhanced point structure for spherical transformations


// Data structure for spherical transformations
typedef struct s_sphere_data
{
    t_point *points;    // Array of points to transform
    int len;           // Number of points
    float radius;      // Sphere radius
    t_vec3 limits;     // Map dimensions/limits
    t_vec3 center;     // Sphere center
    float scale;       // Scale factor
    bool initialized;  // Whether polar coordinates are calculated
} t_sphere_data;

// Transformation callback type
typedef void (*t_transform_callback)(t_sphere_data *data);

// 2D, 3D, 4D Points (alias for vectors)
typedef t_vec2 t_point2;
typedef t_vec3 t_point3;
// Remove or comment out this line to avoid conflict with struct s_point4
// typedef t_vec4 t_point4;

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

// Rectangle (axis-aligned, min/max)
typedef struct 
{
	t_point2	min;
	t_point2	max;
}				t_rect2;

typedef struct s_ipoint
{
	int	x;
	int	y;
	int	z;
}t_ipoint;
// Sphere transformation functions


#endif