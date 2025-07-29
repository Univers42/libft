#include "point.h"
#include "window.h"
#include "input_handler.h"
#include "camera.h"
#include <unistd.h>
#include "mlx.h" // Add this include for mlx_pixel_put and mlx_loop

#define NUM_POINTS 10

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

	// Draw the points in the window using camera projection
	for (int i = 0; i < NUM_POINTS; ++i)
	{
		t_vec2 coord = camera->vtable->project_point(camera, points[i]);
		t_color col = points[i]->vtable->get_color(points[i]);
		mlx_pixel_put(win->mlx, win->win, coord.x, coord.y, col.hex_color);
	}
	input_handler_register(win, handler);
	mlx_loop(win->mlx);

	// Cleanup
	for (int i = 0; i < NUM_POINTS; ++i)
		point_destroy(points[i]);
	camera_destroy(camera);
	win->vtable->destroy(win);
	return 0;
}