/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   point.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 18:46:52 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/28 00:57:54 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POINT_H
#define POINT_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
typedef struct s_vector
{
	int x;
	int y;
	union
	{
		struct
		{
			int z;
		}   s_v3;
		struct
		{
			int z;
			int w;
		}   s_v4;
	}   u_ctx;
}   t_vector;

typedef struct s_position 
{
	double x;
	double y;
	double z;
}   t_position;

typedef struct s_color
{
	uint32_t hex_color;
	struct
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;
	}s_rgba;
}   t_color;

typedef t_vector t_vec2;
typedef t_vector t_vec3;
typedef t_vector t_vec4;

typedef struct s_point t_point;
typedef struct s_point_vtable
{
	t_vec2		(*get_coordinate)(t_point *self);
	t_position	(*get_position)(t_point *self);
	t_color		(*get_color)(t_point *self);
	bool		(*set_coordinate)(t_point *self, int x, int y);
	bool		(*set_position)(t_point *self, double x, double y, double z);
	bool		(*set_color)(t_point *self, uint32_t hex_color);
	bool		(*set_z_value)(t_point *self, int z);
	bool		(*translate)(t_point *self, int dx, int dy);
	bool		(*scale)(t_point *self, double scale_x, double scale_y);
	t_point		*(*clone)(t_point *self);
	void		(*destroy)(t_point *self);
}				t_point_vtable;

struct s_point
{
	t_vec2			coordinate;
	t_position		world_pos;
	t_color			color;
	t_point_vtable	*vtable;
};

/* Constructor and destructor */
t_point		*point_new(int x, int y, int z);
t_point		*point_new_with_color(int x, int y, int z, uint32_t color);
void		point_destroy(t_point *point);

/* Static vtable functions */
t_vec2		point_get_coordinate(t_point *self);
t_position	point_get_position(t_point *self);
t_color		point_get_color(t_point *self);
bool		point_set_coordinate(t_point *self, int x, int y);
bool		point_set_position(t_point *self, double x, double y, double z);
bool		point_set_color(t_point *self, uint32_t hex_color);
bool		point_set_z_value(t_point *self, int z);
bool		point_translate(t_point *self, int dx, int dy);
bool		point_scale(t_point *self, double scale_x, double scale_y);
t_point		*point_clone(t_point *self);
void		point_destroy_method(t_point *self);

/* Utility functions */
uint32_t	rgb_to_hex(uint8_t r, uint8_t g, uint8_t b);
void		hex_to_rgb(uint32_t hex, uint8_t *r, uint8_t *g, uint8_t *b);
double		point_distance(t_point *p1, t_point *p2);
bool		point_equals(t_point *p1, t_point *p2);

#endif