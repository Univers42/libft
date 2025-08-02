/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   point.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 18:46:52 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/01 10:49:25 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POINT_H
# define POINT_H

# include <stdint.h>
# include <stdbool.h>
# include <stdlib.h>
# include "ft_math.h"
# include "window.h"

typedef enum e_terrain
{
	GROUND720 = 0xac7456,
	PURPLE1 = 0Xbfabcc,
	YELLOW_PALE = 0xdecd51,
	GREEN_TERRAIN = 0xaced99,
	PURPLE = 0Xac74f9,
	AQUA = 0x40cdbf,
	SHALLOW = 0X189995,
	DEEP = 0X555999,
	PURPLE2 = 0x60699b,
	PURPLE3 = 0x8879be,
	PURPLE4 = 0xac74f9,
	PINK = 0X997498,
	WHITE1 = 0xdeddcc,
	WHITE2 = 0xe9cab3,
	ORANGE = 0xc56930,
	ORANGE2 = 0Xc66a31,
	ORANGE3 = 0Xd77b34,
	YELLOW_TERRAIN = 0xe79b34,
	GREEN_PALE = 0xb5baa6,
	GREEN_PALE2 = 0xb2b8a5,
	BLUE_TERRAIN = 0X3babbf,
	BLUE2 = 0x3aaaaf,
	BLUE3 = 0X3ca9af,
	BLUE4 = 0x3a98ae
}			t_terrain;

typedef enum e_gamma
{
	gamma1 = 0xac74f9,
	gamma2 = 0x997498
}			t_gamma;

typedef struct s_position
{
	double	x;
	double	y;
	double	z;
}	t_position;

typedef struct s_simple_pos
{
	int	x;
	int	y;
	int	z;
}		t_pos;

typedef struct s_color
{
	uint32_t	hex_color;
	struct
	{
		uint8_t	r;
		uint8_t	g;
		uint8_t	b;
		uint8_t	a;
	}	s_rgba;
}	t_color;

typedef struct s_vectorN
{
	double	x;
	double	y;
	union
	{
		struct
		{
			double	z;
		}	s_v3;
		struct
		{
			double	z;
			double	w;
		}	s_v4;
	}	u_ctx;
}	t_vectorN;

typedef t_vectorN		t_vec2;
typedef t_vectorN		t_vec3;
typedef t_vectorN		t_vec4;

typedef struct s_point	t_point;

// Forward declaration for t_line to resolve circular dependency
struct s_line;
typedef struct s_line t_line;

typedef struct s_point_vtable
{
	void	(*destroy)(t_point *self);
	void	(*get_coordinate)(t_point *self, t_vec2 *out);
	void	(*get_position)(t_point *self, t_position *out);
	void	(*get_color)(t_point *self, t_color *out);
	int		(*set_coordinate)(t_point *self, int x, int y);
	int		(*set_position)(t_point *self, double x, double y, double z);
	int		(*set_color)(t_point *self, uint32_t hex_color);
	int		(*set_z_value)(t_point *self, int z);
	int		(*translate)(t_point *self, int dx, int dy);
	int		(*scale)(t_point *self, double scale_x, double scale_y);
	t_point	*(*clone)(t_point *self);
}				t_point_vtable;

struct s_point
{
	t_vec2			coordinate;
	t_position		world_pos;
	t_color			color;
	t_point_vtable	*vtable;
	t_pos			pos;
	double			speed; // Add speed member
};

/* Constructor and destructor */
t_point		*point_new(int x, int y, int z);
t_point		*point_new_with_color(int x, int y, int z, uint32_t color);
void		point_destroy(t_point *point);

/* Static vtable functions */
void		point_get_coordinate(t_point *self, t_vec2 *out);
void		point_get_position(t_point *self, t_position *out);
void		point_get_color(t_point *self, t_color *out);
int			point_set_coordinate(t_point *self, int x, int y);
int			point_set_position(t_point *self, double x, double y, double z);
int			point_set_color(t_point *self, uint32_t hex_color);
int			point_set_z_value(t_point *self, int z);
int			point_translate(t_point *self, int dx, int dy);
int			point_scale(t_point *self, double scale_x, double scale_y);
t_point		*point_clone(t_point *self);
void		point_destroy_method(t_point *self);

/* Utility functions */
uint32_t	rgb_to_hex(uint8_t r, uint8_t g, uint8_t b);
void		hex_to_rgb(uint32_t hex, uint8_t *r, uint8_t *g, uint8_t *b);
double		euclidean_dist(t_point *p1, t_point *p2);
bool		point_equals(t_point *p1, t_point *p2);

#endif