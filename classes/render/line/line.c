#include "line.h"
#include "ft_math.h"
#include <stdlib.h>

/* Static vtable instance */
static t_line_vtable	g_line_vtable = {
	.get_start = line_get_start,
	.get_end = line_get_end,
	.set_start = line_set_start,
	.set_end = line_set_end,
	.set_points = line_set_points,
	.get_length = line_get_length,
	.get_slope = line_get_slope,
	.is_vertical = line_is_vertical,
	.is_horizontal = line_is_horizontal,
	.interpolate_color = line_interpolate_color,
	.interpolate_point = line_interpolate_point,
	.draw_bresenham = line_draw_bresenham,
	.draw_dda = line_draw_dda,
	.translate = line_translate,
	.scale = line_scale,
	.clone = line_clone,
	.destroy = line_destroy_method
};



uint32_t	line_interpolate_color(t_line *self, double t)
{
	t_color	start_color;
	t_color	end_color;

	if (!self || !self->start || !self->end || t < 0.0 || t > 1.0)
		return (0);
	start_color = self->start->vtable->get_color(self->start);
	end_color = self->end->vtable->get_color(self->end);
	return (color_lerp(start_color.hex_color, end_color.hex_color, t));
}






/* Utility functions */
