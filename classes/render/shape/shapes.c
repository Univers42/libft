/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shapes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:01:34 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/28 11:25:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shapes.h"
#include "drawing.h"
#include "fdf.h"
#include <string.h>
#include <time.h>

// Forward declaration for drawing functions
void draw_circle_enhanced(t_server *srv, t_position center, double radius, int color, bool filled);
void draw_line(t_server *srv, t_coord p1, t_coord p2, int color);

// =============================================================================
// Utility functions
// =============================================================================

t_position make_position_2d(double x, double y) {
	return (t_position){x, y, 0.0};
}

t_position make_position_3d(double x, double y, double z) {
	return (t_position){x, y, z};
}

// Helper: convert t_position to t_coord
static t_coord pos_to_coord(t_position pos) {
	return (t_coord){(int)pos.x, (int)pos.y, (int)pos.z};
}

// Initialize default properties
static t_shape_properties default_properties(void) {
	t_shape_properties props = {0};
	props.visible = true;
	props.filled = true;
	props.fill_color = 0xFFFFFF;
	props.border_color = 0xFFFFFF;
	props.border_width = 1.0;
	props.opacity = 1.0;
	props.name = NULL;
	props.user_data = NULL;
	return props;
}

// Initialize default transform
static t_transform default_transform(void) {
	t_transform trans = {0};
	// Identity matrix
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			trans.matrix[i][j] = (i == j) ? 1.0 : 0.0;
		}
	}
	trans.scale[0] = trans.scale[1] = trans.scale[2] = 1.0;
	return trans;
}

// =============================================================================
// Generic shape operations using vtable
// =============================================================================

void shape_draw(const t_shape *shape, void *srv, void *win) {
	if (!shape || !shape->vtable || !shape->vtable->draw)
		return;
	if (!shape->properties.visible)
		return;
	shape->vtable->draw(shape, srv, win);
}

void shape_destroy(t_shape *shape) {
	if (!shape || !shape->vtable || !shape->vtable->destroy)
		return;
	if (shape->properties.name)
		free(shape->properties.name);
	shape->vtable->destroy(shape);
}

t_shape *shape_clone(const t_shape *shape) {
	if (!shape || !shape->vtable || !shape->vtable->clone)
		return NULL;
	return shape->vtable->clone((t_shape *)shape);
}

// Property setters using vtable
t_shape_error shape_set_position(t_shape *shape, t_position pos) {
	if (!shape || !shape->vtable)
		return SHAPE_ERROR_NULL_POINTER;
	if (shape->vtable->set_position)
		return shape->vtable->set_position(shape, pos);
	shape->position = pos;
	return SHAPE_SUCCESS;
}

t_shape_error shape_set_color(t_shape *shape, int color) {
	if (!shape || !shape->vtable)
		return SHAPE_ERROR_NULL_POINTER;
	if (shape->vtable->set_color)
		return shape->vtable->set_color(shape, color);
	shape->color = color;
	shape->properties.fill_color = color;
	shape->properties.border_color = color;
	return SHAPE_SUCCESS;
}

// Property getters using vtable
t_position shape_get_position(const t_shape *shape) {
	if (!shape)
		return (t_position){0, 0, 0};
	if (shape->vtable && shape->vtable->get_position)
		return shape->vtable->get_position(shape);
	return shape->position;
}

int shape_get_color(const t_shape *shape) {
	if (!shape)
		return 0;
	if (shape->vtable && shape->vtable->get_color)
		return shape->vtable->get_color(shape);
	return shape->color;
}

const char *shape_get_type_name(const t_shape *shape) {
	if (!shape)
		return "invalid";
	if (shape->vtable && shape->vtable->get_type_name)
		return shape->vtable->get_type_name(shape);
	
	switch (shape->type) {
		case SHAPE_2D_CIRCLE: return "circle";
		case SHAPE_2D_SQUARE: return "square";
		case SHAPE_2D_TRIANGLE: return "triangle";
		case SHAPE_2D_LINE: return "line";
		case SHAPE_2D_STAR: return "star";
		case SHAPE_3D_SPHERE: return "sphere";
		case SHAPE_3D_CUBE: return "cube";
		default: return "unknown";
	}
}

t_shape_type shape_get_type(const t_shape *shape) {
	if (!shape)
		return SHAPE_INVALID;
	return shape->type;
}

bool shape_is_2d(const t_shape *shape) {
	if (!shape)
		return false;
	return shape->type >= SHAPE_2D_LINE && shape->type <= SHAPE_2D_SPIRAL;
}

bool shape_is_3d(const t_shape *shape) {
	if (!shape)
		return false;
	return shape->type >= SHAPE_3D_CUBE && shape->type <= SHAPE_3D_PYRAMID;
}

// =============================================================================
// CIRCLE implementation
// =============================================================================

static void circle_draw(const t_shape *self, void *srv, void *win) {
	(void)win;
	if (!self || !srv)
		return;
	draw_circle_enhanced((t_server *)srv, self->position, 
						self->params.s_circle.radius, 
						self->properties.filled ? self->properties.fill_color : self->properties.border_color,
						self->properties.filled);
}

static void circle_destroy(t_shape *self) {
	if (self) {
		if (self->properties.name)
			free(self->properties.name);
		free(self);
	}
}

static t_shape *circle_clone(const t_shape *self) {
	if (!self)
		return NULL;
	return new_circle(self->position, self->color, self->params.s_circle.radius);
}

static const char *circle_get_type_name(const t_shape *self) {
	(void)self;
	return "circle";
}

static double circle_get_area(const t_shape *self) {
	if (!self)
		return 0.0;
	double r = self->params.s_circle.radius;
	return M_PI * r * r;
}

static double circle_get_perimeter(const t_shape *self) {
	if (!self)
		return 0.0;
	return 2.0 * M_PI * self->params.s_circle.radius;
}

static const t_shape_vtable g_circle_vtable = {
	.draw = circle_draw,
	.destroy = circle_destroy,
	.clone = circle_clone,
	.get_type_name = circle_get_type_name,
	.get_area = circle_get_area,
	.get_perimeter = circle_get_perimeter,
};

t_shape *new_circle(t_position pos, int color, double radius) {
	t_shape *shape = malloc(sizeof(t_shape));
	if (!shape)
		return NULL;
	
	memset(shape, 0, sizeof(t_shape));
	shape->vtable = &g_circle_vtable;
	shape->type = SHAPE_2D_CIRCLE;
	shape->position = pos;
	shape->color = color;
	shape->params.s_circle.radius = radius;
	shape->properties = default_properties();
	shape->properties.fill_color = color;
	shape->properties.border_color = color;
	shape->transform = default_transform();
	shape->creation_time = time(NULL);
	strcpy(shape->class_name, "Circle");
	
	return shape;
}

// =============================================================================
// SQUARE implementation
// =============================================================================

static void square_draw(const t_shape *self, void *srv, void *win) {
	(void)win;
	if (!self || !srv)
		return;
	
	t_coord c = pos_to_coord(self->position);
	int size = (int)self->params.s_square.size;
	int color = self->properties.filled ? self->properties.fill_color : self->properties.border_color;
	
	t_coord corners[4] = {
		{c.x, c.y, c.z},
		{c.x + size, c.y, c.z},
		{c.x + size, c.y + size, c.z},
		{c.x, c.y + size, c.z}
	};
	
	// Draw the square outline
	for (int i = 0; i < 4; i++) {
		draw_line((t_server *)srv, corners[i], corners[(i + 1) % 4], color);
	}
	
	// Fill if needed
	if (self->properties.filled) {
		for (int y = 0; y < size; y++) {
			t_coord p1 = {c.x, c.y + y, c.z};
			t_coord p2 = {c.x + size, c.y + y, c.z};
			draw_line((t_server *)srv, p1, p2, color);
		}
	}
}

static void square_destroy(t_shape *self) {
	if (self) {
		if (self->properties.name)
			free(self->properties.name);
		free(self);
	}
}

static t_shape *square_clone(const t_shape *self) {
	if (!self)
		return NULL;
	return new_square(self->position, self->color, self->params.s_square.size);
}

static double square_get_area(const t_shape *self) {
	if (!self)
		return 0.0;
	double s = self->params.s_square.size;
	return s * s;
}

static double square_get_perimeter(const t_shape *self) {
	if (!self)
		return 0.0;
	return 4.0 * self->params.s_square.size;
}

static const char *square_get_type_name(const t_shape *self) {
	(void)self;
	return "square";
}

static const t_shape_vtable g_square_vtable = {
	.draw = square_draw,
	.destroy = square_destroy,
	.clone = square_clone,
	.get_type_name = square_get_type_name,  // Fixed: was referencing circle_get_type_name
	.get_area = square_get_area,
	.get_perimeter = square_get_perimeter,
};

t_shape *new_square(t_position pos, int color, double size) {
	t_shape *shape = malloc(sizeof(t_shape));
	if (!shape)
		return NULL;
	
	memset(shape, 0, sizeof(t_shape));
	shape->vtable = &g_square_vtable;
	shape->type = SHAPE_2D_SQUARE;
	shape->position = pos;
	shape->color = color;
	shape->params.s_square.size = size;
	shape->properties = default_properties();
	shape->properties.fill_color = color;
	shape->properties.border_color = color;
	shape->transform = default_transform();
	shape->creation_time = time(NULL);
	strcpy(shape->class_name, "Square");
	
	return shape;
}

// =============================================================================
// TRIANGLE implementation
// =============================================================================

static void triangle_draw(const t_shape *self, void *srv, void *win) {
	(void)win;
	if (!self || !srv)
		return;
	
	t_coord c = pos_to_coord(self->position);
	int size = (int)self->params.s_triangle.size;
	int color = self->properties.filled ? self->properties.fill_color : self->properties.border_color;
	
	// Calculate triangle points (equilateral triangle)
	t_coord p1 = {c.x, c.y - size/2, c.z};          // Top point
	t_coord p2 = {c.x - size/2, c.y + size/2, c.z}; // Bottom left
	t_coord p3 = {c.x + size/2, c.y + size/2, c.z}; // Bottom right
	
	// Draw triangle outline
	draw_line((t_server *)srv, p1, p2, color);
	draw_line((t_server *)srv, p2, p3, color);
	draw_line((t_server *)srv, p3, p1, color);
}

static void triangle_destroy(t_shape *self) {
	if (self) {
		if (self->properties.name)
			free(self->properties.name);
		free(self);
	}
}

static t_shape *triangle_clone(const t_shape *self) {
	if (!self)
		return NULL;
	return new_triangle(self->position, self->color, self->params.s_triangle.size);
}

static double triangle_get_area(const t_shape *self) {
	if (!self)
		return 0.0;
	double s = self->params.s_triangle.size;
	// Area of equilateral triangle = (sqrt(3)/4) * s^2
	return (sqrt(3.0) / 4.0) * s * s;
}

static const t_shape_vtable g_triangle_vtable = {
	.draw = triangle_draw,
	.destroy = triangle_destroy,
	.clone = triangle_clone,
	.get_area = triangle_get_area,
};

t_shape *new_triangle(t_position pos, int color, double size) {
	t_shape *shape = malloc(sizeof(t_shape));
	if (!shape)
		return NULL;
	
	memset(shape, 0, sizeof(t_shape));
	shape->vtable = &g_triangle_vtable;
	shape->type = SHAPE_2D_TRIANGLE;
	shape->position = pos;
	shape->color = color;
	shape->params.s_triangle.size = size;
	shape->properties = default_properties();
	shape->properties.fill_color = color;
	shape->properties.border_color = color;
	shape->transform = default_transform();
	shape->creation_time = time(NULL);
	strcpy(shape->class_name, "Triangle");
	
	return shape;
}

// =============================================================================
// LINE implementation
// =============================================================================

static void line_draw(const t_shape *self, void *srv, void *win) {
	(void)win;
	if (!self || !srv)
		return;
	
	t_coord start = pos_to_coord(self->position);
	t_coord end = {(int)self->params.line.x1, (int)self->params.line.y1, start.z};
	draw_line((t_server *)srv, start, end, self->color);
}

static void line_destroy(t_shape *self) {
	if (self) {
		if (self->properties.name)
			free(self->properties.name);
		free(self);
	}
}

static t_shape *line_clone(const t_shape *self) {
	if (!self)
		return NULL;
	t_position end = {self->params.line.x1, self->params.line.y1, self->position.z};
	return new_line(self->position, end, self->color);
}

static const t_shape_vtable g_line_vtable = {
	.draw = line_draw,
	.destroy = line_destroy,
	.clone = line_clone,
};

t_shape *new_line(t_position start, t_position end, int color) {
	t_shape *shape = malloc(sizeof(t_shape));
	if (!shape)
		return NULL;
	
	memset(shape, 0, sizeof(t_shape));
	shape->vtable = &g_line_vtable;
	shape->type = SHAPE_2D_LINE;
	shape->position = start;
	shape->color = color;
	shape->params.line.x1 = end.x;
	shape->params.line.y1 = end.y;
	shape->properties = default_properties();
	shape->properties.filled = false; // Lines are not filled
	shape->properties.border_color = color;
	shape->transform = default_transform();
	shape->creation_time = time(NULL);
	strcpy(shape->class_name, "Line");
	
	return shape;
}

// =============================================================================
// STAR implementation
// =============================================================================

static void star_draw(const t_shape *self, void *srv, void *win) {
	(void)win;
	if (!self || !srv)
		return;
	
	t_coord c = pos_to_coord(self->position);
	int color = self->properties.filled ? self->properties.fill_color : self->properties.border_color;
	double outer = self->params.s_star.outer_radius;
	double inner = self->params.s_star.inner_radius;
	
	// Draw a 5-pointed star
	t_coord points[10];
	for (int i = 0; i < 10; i++) {
		double angle = (i * M_PI) / 5.0;
		double radius = (i % 2 == 0) ? outer : inner;
		points[i].x = c.x + (int)(radius * cos(angle));
		points[i].y = c.y + (int)(radius * sin(angle));
		points[i].z = c.z;
	}
	
	// Connect the points
	for (int i = 0; i < 10; i++) {
		draw_line((t_server *)srv, points[i], points[(i + 1) % 10], color);
	}
}

static void star_destroy(t_shape *self) {
	if (self) {
		if (self->properties.name)
			free(self->properties.name);
		free(self);
	}
}

static t_shape *star_clone(const t_shape *self) {
	if (!self)
		return NULL;
	return new_star(self->position, self->color, 
					self->params.s_star.outer_radius, 
					self->params.s_star.inner_radius);
}

static const t_shape_vtable g_star_vtable = {
	.draw = star_draw,
	.destroy = star_destroy,
	.clone = star_clone,
};

t_shape *new_star(t_position pos, int color, double outer, double inner) {
	t_shape *shape = malloc(sizeof(t_shape));
	if (!shape)
		return NULL;
	
	memset(shape, 0, sizeof(t_shape));
	shape->vtable = &g_star_vtable;
	shape->type = SHAPE_2D_STAR;
	shape->position = pos;
	shape->color = color;
	shape->params.s_star.outer_radius = outer;
	shape->params.s_star.inner_radius = inner;
	shape->properties = default_properties();
	shape->properties.fill_color = color;
	shape->properties.border_color = color;
	shape->transform = default_transform();
	shape->creation_time = time(NULL);
	strcpy(shape->class_name, "Star");
	
	return shape;
}

// =============================================================================
// SPHERE implementation (3D)
// =============================================================================

static void sphere_draw(const t_shape *self, void *srv, void *win) {
	(void)win;
	if (!self || !srv)
		return;
	
	// For 3D sphere, we'll draw it as a circle with some 3D effect
	// In a real 3D engine, this would involve proper projection
	t_position projected = self->position;
	projected.x += self->position.z * 0.3; // Simple 3D projection
	projected.y += self->position.z * 0.2;
	
	draw_circle_enhanced((t_server *)srv, projected, 
						self->params.s_sphere.radius,
						self->properties.filled ? self->properties.fill_color : self->properties.border_color,
						self->properties.filled);
}

static void sphere_destroy(t_shape *self) {
	if (self) {
		if (self->properties.name)
			free(self->properties.name);
		free(self);
	}
}

static t_shape *sphere_clone(const t_shape *self) {
	if (!self)
		return NULL;
	return new_sphere(self->position, self->color, self->params.s_sphere.radius);
}

static double sphere_get_area(const t_shape *self) {
	if (!self)
		return 0.0;
	double r = self->params.s_sphere.radius;
	return 4.0 * M_PI * r * r; // Surface area of sphere
}

static const t_shape_vtable g_sphere_vtable = {
	.draw = sphere_draw,
	.destroy = sphere_destroy,
	.clone = sphere_clone,
	.get_area = sphere_get_area,
};

t_shape *new_sphere(t_position pos, int color, double radius) {
	t_shape *shape = malloc(sizeof(t_shape));
	if (!shape)
		return NULL;
	
	memset(shape, 0, sizeof(t_shape));
	shape->vtable = &g_sphere_vtable;
	shape->type = SHAPE_3D_SPHERE;
	shape->position = pos;
	shape->color = color;
	shape->params.s_sphere.radius = radius;
	shape->properties = default_properties();
	shape->properties.fill_color = color;
	shape->properties.border_color = color;
	shape->transform = default_transform();
	shape->creation_time = time(NULL);
	strcpy(shape->class_name, "Sphere");
	
	return shape;
}

// =============================================================================
// CUBE implementation (3D)
// =============================================================================

static void cube_draw(const t_shape *self, void *srv, void *win) {
	(void)win;
	if (!self || !srv)
		return;
	
	t_coord c = pos_to_coord(self->position);
	int size = (int)self->params.s_cube.size;
	int color = self->properties.filled ? self->properties.fill_color : self->properties.border_color;
	
	// Draw front face
	t_coord front[4] = {
		{c.x, c.y, c.z},
		{c.x + size, c.y, c.z},
		{c.x + size, c.y + size, c.z},
		{c.x, c.y + size, c.z}
	};
	
	// Draw back face (with 3D offset)
	int offset = size / 3;
	t_coord back[4] = {
		{c.x + offset, c.y - offset, c.z},
		{c.x + size + offset, c.y - offset, c.z},
		{c.x + size + offset, c.y + size - offset, c.z},
		{c.x + offset, c.y + size - offset, c.z}
	};
	
	// Draw front face
	for (int i = 0; i < 4; i++) {
		draw_line((t_server *)srv, front[i], front[(i + 1) % 4], color);
	}
	
	// Draw back face
	for (int i = 0; i < 4; i++) {
		draw_line((t_server *)srv, back[i], back[(i + 1) % 4], color);
	}
	
	// Connect front and back
	for (int i = 0; i < 4; i++) {
		draw_line((t_server *)srv, front[i], back[i], color);
	}
}

static void cube_destroy(t_shape *self) {
	if (self) {
		if (self->properties.name)
			free(self->properties.name);
		free(self);
	}
}

static t_shape *cube_clone(const t_shape *self) {
	if (!self)
		return NULL;
	return new_cube(self->position, self->color, self->params.s_cube.size);
}

static double cube_get_area(const t_shape *self) {
	if (!self)
		return 0.0;
	double s = self->params.s_cube.size;
	return 6.0 * s * s; // Surface area of cube
}

static const t_shape_vtable g_cube_vtable = {
	.draw = cube_draw,
	.destroy = cube_destroy,
	.clone = cube_clone,
	.get_area = cube_get_area,
};

t_shape *new_cube(t_position pos, int color, double size) {
	t_shape *shape = malloc(sizeof(t_shape));
	if (!shape)
		return NULL;
	
	memset(shape, 0, sizeof(t_shape));
	shape->vtable = &g_cube_vtable;
	shape->type = SHAPE_3D_CUBE;
	shape->position = pos;
	shape->color = color;
	shape->params.s_cube.size = size;
	shape->properties = default_properties();
	shape->properties.fill_color = color;
	shape->properties.border_color = color;
	shape->transform = default_transform();
	shape->creation_time = time(NULL);
	strcpy(shape->class_name, "Cube");
	
	return shape;
}

// =============================================================================
// DIAMOND implementation
// =============================================================================

static void diamond_draw(const t_shape *self, void *srv, void *win) {
	(void)win;
	if (!self || !srv)
		return;
	
	t_coord c = pos_to_coord(self->position);
	int size = (int)self->params.s_diamond.size;
	int color = self->properties.filled ? self->properties.fill_color : self->properties.border_color;
	
	// Diamond points
	t_coord points[4] = {
		{c.x, c.y - size/2, c.z},        // Top
		{c.x + size/2, c.y, c.z},        // Right
		{c.x, c.y + size/2, c.z},        // Bottom
		{c.x - size/2, c.y, c.z}         // Left
	};
	
	// Draw diamond outline
	for (int i = 0; i < 4; i++) {
		draw_line((t_server *)srv, points[i], points[(i + 1) % 4], color);
	}
}

static void diamond_destroy(t_shape *self) {
	if (self) {
		if (self->properties.name)
			free(self->properties.name);
		free(self);
	}
}

static t_shape *diamond_clone(const t_shape *self) {
	if (!self)
		return NULL;
	return new_diamond(self->position, self->color, self->params.s_diamond.size);
}

static const t_shape_vtable g_diamond_vtable = {
	.draw = diamond_draw,
	.destroy = diamond_destroy,
	.clone = diamond_clone,
};

t_shape *new_diamond(t_position pos, int color, double size) {
	t_shape *shape = malloc(sizeof(t_shape));
	if (!shape)
		return NULL;
	
	memset(shape, 0, sizeof(t_shape));
	shape->vtable = &g_diamond_vtable;
	shape->type = SHAPE_2D_DIAMOND;
	shape->position = pos;
	shape->color = color;
	shape->params.s_diamond.size = size;
	shape->properties = default_properties();
	shape->properties.fill_color = color;
	shape->properties.border_color = color;
	shape->transform = default_transform();
	shape->creation_time = time(NULL);
	strcpy(shape->class_name, "Diamond");
	
	return shape;
}

// =============================================================================
// Parameter makers
// =============================================================================

t_shape_params make_line_params(double x1, double y1) {
	t_shape_params params = {0};
	params.line.x1 = x1;
	params.line.y1 = y1;
	return params;
}

t_shape_params make_size_params(double size) {
	t_shape_params params = {0};
	params.s_square.size = size; // Can be used for any size-based shape
	return params;
}

t_shape_params make_radius_params(double radius) {
	t_shape_params params = {0};
	params.s_circle.radius = radius; // Can be used for any radius-based shape
	return params;
}

t_shape_params make_star_params(double outer, double inner) {
	t_shape_params params = {0};
	params.s_star.outer_radius = outer;
	params.s_star.inner_radius = inner;
	return params;
}

t_shape_params make_spiral_params(double turns, double max_radius) {
	t_shape_params params = {0};
	params.s_spiral.turns = turns;
	params.s_spiral.max_radius = max_radius;
	return params;
}
