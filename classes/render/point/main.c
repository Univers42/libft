#include "point.h"
#include "window.h"
#include "input_handler.h"
#include "camera.h"
#include <unistd.h>
#include "mlx.h" // Add this include for mlx_pixel_put and mlx_loop

#define NUM_POINTS 10

// Helper to draw a filled square centered at (x, y) with given size into the image buffer
static void draw_filled_square_to_buffer(t_window *win, int x, int y, int size, uint32_t color)
{
	int bpp, size_line, endian;
	char *buf = win->vtable->get_image_buffer(win, &bpp, &size_line, &endian);
	if (!buf) return;
	int half = size / 2;
	for (int dy = -half; dy <= half; ++dy)
	{
		int py = y + dy;
		if (py < 0 || py >= win->height) continue;
		for (int dx = -half; dx <= half; ++dx)
		{
			int px = x + dx;
			if (px < 0 || px >= win->width) continue;
			int offset = py * size_line + px * (bpp / 8);
			*(uint32_t *)(buf + offset) = color;
		}
	}
}

int main(void)
{
	t_window *win = window_new(800, 600, "Point Demo");
	if (!win)
		return (1);

	t_camera *camera = camera_new(CAMERA_ISOMETRIC);
	if (!camera)
		return (1);

	t_input_handler *handler = InputHandler_new(camera); // Pass camera
	if (!handler)
		return (1);
	// Create points at different positions with different colors
	t_point *points[NUM_POINTS];
	for (int i = 0; i < NUM_POINTS; ++i)
	{
		int x = 50 + i * 70;
		int y = 100 + (i % 2) * 200;
		uint32_t color = rgb_to_hex(20 * i, 255 - 20 * i, 100 + 10 * i);
		points[i] = point_new_with_color(x, y, 0, color);
	}

	// Clear image buffer
	int bpp, size_line, endian;
	char *buf = win->vtable->get_image_buffer(win, &bpp, &size_line, &endian);
	if (buf)
		ft_memset(buf, 0, size_line * win->height);

	// Draw the points in the window using camera projection
	for (int i = 0; i < NUM_POINTS; ++i)
	{
		t_vec2 coord = camera->vtable->project_point(camera, points[i]);
		t_color col = points[i]->vtable->get_color(points[i]);
		int size = (int)(camera->zoom * 4.0);
		if (size < 1) size = 1;
		draw_filled_square_to_buffer(win, coord.x, coord.y, size, col.hex_color);
	}
	win->vtable->update_image(win);

	input_handler_register(win, handler);
	mlx_loop(win->mlx);

	// Cleanup
	for (int i = 0; i < NUM_POINTS; ++i)
		point_destroy(points[i]);
	camera_destroy(camera);
	win->vtable->destroy(win);
	return 0;
}