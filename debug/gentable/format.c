#include "database.h"

// ============= CELL FORMATTING =============

void	print_rgb_color(t_color c)
{
	ft_printf("\033[38;2;%d;%d;%dm", (int)c.u_raw_color.r,
		(int)c.u_raw_color.g, (int)c.u_raw_color.b);
}

void	print_rgb_bg_color(t_color c)
{
	ft_printf("\033[48;2;%d;%d;%dm", (int)c.u_raw_color.r,
		(int)c.u_raw_color.g, (int)c.u_raw_color.b);
}

t_color	make_color(uint8_t r, uint8_t g, uint8_t b)
{
	t_color	c;

	c.z_val = 0;
	c.zf_val = 0.0f;
	c.hex_color = ((uint32_t)r << 16) | ((uint32_t)g << 8) | (uint32_t)b;
	c.u_raw_color.r = r;
	c.u_raw_color.g = g;
	c.u_raw_color.b = b;
	c.u_raw_color.a = 255;
	return (c);
}

// alias for make_color currently; kept for clarity
t_color	make_background(uint8_t r, uint8_t g, uint8_t b)
{
	return make_color(r, g, b);
}
