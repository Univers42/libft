/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 18:49:14 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/01 08:54:22 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LINE_H
# define LINE_H
# include <stdint.h>
# include "point.h"

typedef struct s_line t_line;

typedef void (*t_pixel_callback)(int x, int y, uint32_t color, void *data);

typedef struct s_line_vtable
{
	t_point		*(*get_start)(t_line *self);
	t_point		*(*get_end)(t_line *self);
	bool		(*set_start)(t_line *self, t_point *start);
	bool		(*set_end)(t_line *self, t_point *end);
	bool		(*set_points)(t_line *self, t_point *start, t_point *end);
	double		(*get_length)(t_line *self);
	double		(*get_slope)(t_line *self);
	bool		(*is_vertical)(t_line *self);
	bool		(*is_horizontal)(t_line *self);
	uint32_t	(*interpolate_color)(t_line *self, double t);
	t_point		*(*interpolate_point)(t_line *self, double t);
	void		(*draw_bresenham)(t_line *self, t_pixel_callback callback,
				void *data);
	void		(*draw_dda)(t_line *self, t_pixel_callback callback,
				void *data);
	bool		(*translate)(t_line *self, int dx, int dy);
	bool		(*scale)(t_line *self, double scale_x, double scale_y);
	t_line		*(*clone)(t_line *self);
	void		(*destroy)(t_line *self);
}				t_line_vtable;

typedef struct s_line
{
	t_point			*start;
	t_point			*end;
	bool			owns_points;
	t_line_vtable	*vtable;
}					t_line;

/* Constructors and destructor */
t_line		*line_new(t_point *start, t_point *end);
t_line		*line_new_owned(t_point *start, t_point *end);
t_line		*line_new_coordinates(int x1, int y1, int z1,
				int x2, int y2, int z2);
t_line		*line_new_with_colors(int x1, int y1, int z1,
				uint32_t color1, int x2, int y2, int z2,
				uint32_t color2);
void		line_destroy(t_line *line);

/* Static vtable functions */
t_point		*line_get_start(t_line *self);
t_point		*line_get_end(t_line *self);
bool		line_set_start(t_line *self, t_point *start);
bool		line_set_end(t_line *self, t_point *end);
bool		line_set_points(t_line *self, t_point *start, t_point *end);
double		line_get_length(t_line *self);
double		line_get_slope(t_line *self);
bool		line_is_vertical(t_line *self);
bool		line_is_horizontal(t_line *self);
uint32_t	line_interpolate_color(t_line *self, double t);
t_point		*line_interpolate_point(t_line *self, double t);
void		line_draw_bresenham(t_line *self, t_pixel_callback callback,
				void *data);
void		line_draw_dda(t_line *self, t_pixel_callback callback,
				void *data);
bool		line_translate(t_line *self, int dx, int dy);
bool		line_scale(t_line *self, double scale_x, double scale_y);
t_line		*line_clone(t_line *self);
void		line_destroy_method(t_line *self);

/* Utility functions */
bool		line_intersects(t_line *line1, t_line *line2,
				t_point **intersection);
double		line_distance_to_point(t_line *line, t_point *point);
bool		line_equals(t_line *line1, t_line *line2);
uint32_t	color_lerp(uint32_t color1, uint32_t color2, double t);

#endif