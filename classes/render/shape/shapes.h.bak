/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shapes.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:01:29 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/28 11:01:30 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHAPES_H
#define SHAPES_H
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Forward declarations
typedef struct s_shape t_shape;
typedef struct s_shape_vtable t_shape_vtable;
typedef struct s_shape_class t_shape_class;

// Position struct must be defined before use
typedef struct s_position {
	double x;
	double y;
	double z;
} t_position;


// Shape parameters union for different shape types
typedef union u_shape_params
{
	struct
	{
		double	x1;
		double	y1; // end point for line
	}	line;
	struct
	{
		double	size;
	}	s_square;
	struct
	{
		double	size;
	}	s_triangle;
	struct {
		double	size;
	} s_diamond;
	struct
	{
		double	radius;
	}s_circle;
	struct
	{
		double	size;
	}	s_heart;
	struct
	{
		double	radius;
	}	s_pentagon;
	struct
	{
		double	radius;
	}	s_hexagon;
	struct
	{
		double	outer_radius;
		double	inner_radius;
	}	s_star;
	struct
	{
		double	turns;
		double	max_radius;
	}	s_spiral;
	struct
	{
		double	size;
	}	s_cube;
	struct
	{
		double	radius;
	}	s_sphere;
	struct
	{
		double	size;
	}	s_pyramid;
}	t_shape_params;

typedef enum e_shape_type
{
	SHAPE_INVALID = -1,
	SHAPE_2D_LINE = 0,
	SHAPE_2D_SQUARE,
	SHAPE_2D_TRIANGLE,
	SHAPE_2D_DIAMOND,
	SHAPE_2D_CIRCLE,
	SHAPE_2D_HEART,
	SHAPE_2D_PENTAGON,
	SHAPE_2D_HEXAGON,
	SHAPE_2D_STAR,
	SHAPE_2D_SPIRAL,
	SHAPE_3D_CUBE,
	SHAPE_3D_SPHERE,
	SHAPE_3D_PYRAMID
}	t_shape_type;

typedef enum e_render_mode 
{
	RENDER_2D = 0,
	RENDER_3D,
	RENDER_MODE_COUNT
}	t_render_mode;

typedef enum e_shape_error
{
	SHAPE_SUCCESS = 0,
	SHAPE_ERROR_NULL_POINTER,
	SHAPE_ERROR_INVALID_TYPE,
	SHAPE_ERROR_MEMORY_ALLOCATION,
	SHAPE_ERROR_INVALID_PARAMETER,
	SHAPE_ERROR_NOT_IMPLEMENTED
}	t_shape_error;

// Bounding box for collision detection and optimization
typedef struct s_collision
{
	t_position	min;
	t_position	max;
}	t_collision;

typedef struct s_transform
{
	double		matrix[4][4];
	double		rotation[3];
	double		scale[3];
	t_position	translation;
 }				t_transform;

typedef struct s_shape_properties
{
	bool	visible;
	bool	filled;
	int		fill_color;
	int		border_color;
	double	border_width;
	double	opacity;
	char	*name;
	void	*user_data;
}			t_shape_properties;

typedef struct	s_shape_vtable
{
	// Core polymorphic methods
	void (*draw)					(const t_shape *self, void *srv, void *win);
	void (*destroy)					(t_shape *self);

	// Canonical form methods: The orthodox way
	t_shape *(*clone)				(const t_shape *self);
	t_shape *(*assign)				(t_shape *self, const t_shape *other);

	// Comparison operators
	bool (*equals)					(const t_shape *self, const t_shape *other);
	int (*compare)					(const t_shape *self, const t_shape *other);

	// Accessors and mutators (setters/getters)
	t_shape_error (*set_position)	(t_shape *self, t_position pos);
	t_shape_error (*set_color)		(t_shape *self, int color);
	t_shape_error (*set_params)		(t_shape *self, t_shape_params params);
	t_shape_error (*set_transform)	(t_shape *self, const t_transform *transform);

	// Getters
	t_position (*get_position)		(const t_shape *self);
	int (*get_color)				(const t_shape *self);
	t_shape_params (*get_params)	(const t_shape *self);
	t_transform (*get_transform)	(const t_shape *self);
	const char *(*get_type_name)	(const t_shape *self);

	// Type information (RTTI-like)
	t_shape_type (*get_type)		(const t_shape *self);
	size_t (*get_size)				(const t_shape *self);
	t_collision (*get_bounding_box)	(const t_shape *self);
	double (*get_area)				(const t_shape *self);
	double (*get_perimeter)			(const t_shape *self);
	bool (*contains_point)			(const t_shape *self, t_position point);
	bool (*intersects)				(const t_shape *self, const t_shape *other);

	// Serialization
	char *(*to_string)				(const t_shape *self);
	t_shape_error (*from_string)	(t_shape *self, const char *str);

	// Animation and transformation
	void (*rotate)					(t_shape *self, double angle_x, double angle_y, double angle_z);
	void (*scale)					(t_shape *self, double scale_x, double scale_y, double scale_z);
	void (*translate)				(t_shape *self, double dx, double dy, double dz);
}									t_shape_vtable;

// Mask/flags for constructor
typedef enum e_shape_ctor_mask
{
	SHAPE_CTOR_NONE = 0,
	SHAPE_CTOR_POSITION = 1 << 0,
	SHAPE_CTOR_COLOR = 1 << 1,
	SHAPE_CTOR_PARAMS = 1 << 2,
	SHAPE_CTOR_RENDER_MODE = 1 << 3,
	SHAPE_CTOR_PROPERTIES = 1 << 4,
	SHAPE_CTOR_TRANSFORM = 1 << 5,
	SHAPE_CTOR_ALL = 0xFFFFFFFF
}	t_shape_ctor_mask;

// Struct to group constructor arguments
typedef struct s_shape_ctor_args
{
	const t_position			*pos;
	const int					*color;
	const t_shape_params		*params;
	const t_render_mode			*render_mode;
	const t_shape_properties	*properties;
	const t_transform			*transform;
} 								t_shape_ctor_args;

typedef struct s_shape_class
{
	const char								*class_name;
	t_shape_type							type;
	size_t									instance_size;
	// Flexible constructor with mask and grouped args
	t_shape *(*construct)					(t_shape_ctor_mask mask, const t_shape_ctor_args *args);
	// Class methods (static like)
	bool (*is_valid_params)					(t_shape_params params);
	t_shape_params (*get_default_params)	(void);
	const char *(*get_class_description)	(void);
	t_shape_error (*register_class)			(void);
	void (*unregister_class)				(void);
}											t_shape_class;

// Main shape structure
struct s_shape
{
	const t_shape_class		*class_info;
	const t_shape_vtable	*vtable;

	// Core properties
	t_shape_type			type;
	t_position				position;
	int						color;
	t_shape_params			params;
	t_render_mode			render_mode;
	t_shape_properties		properties;
	t_transform				transform;
	t_collision				cached_bbox;

	// Memory management
	size_t					ref_count;
	bool					bbox_valid;
	// Metadata
	unsigned long			creation_time;
	unsigned long			last_modified;
	char					class_name[64];
};

// =============================================================================
// Polymorphic Interface (using vtable)
// =============================================================================

// Core methods that use vtable
void			shape_draw(const t_shape *shape, void *srv, void *win);
void			shape_destroy(t_shape *shape);
t_shape			*shape_clone(const t_shape *shape);

// Property accessors with error handling
t_shape_error	shape_set_position(t_shape *shape, t_position pos);
t_shape_error	shape_set_color(t_shape *shape, int color);
t_shape_error	shape_set_params(t_shape *shape, t_shape_params params);
t_shape_error	shape_set_render_mode(t_shape *shape, t_render_mode mode);

// Property getters (const-correct)
t_position		shape_get_position(const t_shape *shape);
int				shape_get_color(const t_shape *shape);
t_shape_params	shape_get_params(const t_shape *shape);
t_render_mode	shape_get_render_mode(const t_shape *shape);

// Type information (RTTI-like)
const char		*shape_get_type_name(const t_shape *shape);
t_shape_type	shape_get_type(const t_shape *shape);
size_t			shape_get_size(const t_shape *shape);
bool			shape_is_2d(const t_shape *shape);
bool			shape_is_3d(const t_shape *shape);

// =============================================================================
// Advanced methods
// =============================================================================

// Geometric calculations
t_collision		shape_get_bounding_box(const t_shape *shape);
double			shape_get_area(const t_shape *shape);
double			shape_get_perimeter(const t_shape *shape);
bool			shape_contains_point(const t_shape *shape, t_position point);
bool			shape_intersects(const t_shape *a, const t_shape *b);

// Utility functions
t_position		make_position_2d(double x, double y);
t_position		make_position_3d(double x, double y, double z);

// Shape factory functions
t_shape			*new_circle(t_position pos, int color, double radius);
t_shape			*new_square(t_position pos, int color, double size);
t_shape			*new_triangle(t_position pos, int color, double size);
t_shape			*new_line(t_position start, t_position end, int color);
t_shape			*new_star(t_position pos, int color, double outer, double inner);
t_shape			*new_sphere(t_position pos, int color, double radius);
t_shape			*new_cube(t_position pos, int color, double size);
t_shape			*new_diamond(t_position pos, int color, double size);
t_shape			*new_heart(t_position pos, int color, double size);
t_shape			*new_pentagon(t_position pos, int color, double radius);
t_shape			*new_hexagon(t_position pos, int color, double radius);
t_shape			*new_spiral(t_position pos, int color, double turns, double max_radius);

// Shape parameter makers
t_shape_params	make_line_params(double x1, double y1);
t_shape_params	make_size_params(double size);
t_shape_params	make_radius_params(double radius);
t_shape_params	make_star_params(double outer, double inner);
t_shape_params	make_spiral_params(double turns, double max_radius);

// Error handling
const char		*shape_error_to_string(t_shape_error error);
bool			shape_is_valid(const t_shape *shape);

#endif


shape->line.fn();

shape->line->point.fn();


t_point p;

p->