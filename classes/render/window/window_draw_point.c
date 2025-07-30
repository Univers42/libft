#include "window.h"
#include <stdint.h>

void window_draw_point(t_window *win, int x, int y, unsigned int color)
{
	if (!win || !win->img || x < 0 || y < 0 || x >= win->width || y >= win->height)
		return;

	int bpp, size_line, endian;
	char *buf = win->vtable->get_image_buffer(win, &bpp, &size_line, &endian);
	if (!buf)
		return;

	int offset = y * size_line + x * (bpp / 8);
	*(uint32_t *)(buf + offset) = color;
}

// Helper callback for line drawing
static void	window_pixel_callback(int x, int y, uint32_t color, void *data)
{
	t_window *win = (t_window *)data;
	window_draw_point(win, x, y, color);
}

void	window_draw_line(t_window *win, t_line *line)
{
	if (!win || !line)
		return;
	line->vtable->draw_bresenham(line, window_pixel_callback, win);
}
