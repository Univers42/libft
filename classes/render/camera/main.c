#include "window.h"
#include "point.h"
#include "camera.h"
#include "input_handler.h"
#include <unistd.h>
#include "mlx.h"
#include <X11/Xlib.h>      // Add this line
#include "mlx_int.h"       // Add this line for t_xvar and t_win_list

#define NUM_POINTS 5

typedef struct s_app
{
	t_window *win;
	t_camera *camera;
	t_point *points[NUM_POINTS];
} t_app;

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

static int redraw(void *param)
{
	t_app *app = (t_app *)param;

	// --- Poll window size and resize image buffer if needed ---
	int actual_width, actual_height;
	mlx_get_screen_size(app->win->mlx, &actual_width, &actual_height); // fallback, not always reliable
	// Instead, use X11 directly to get the window size:
	Window root;
	int x, y;
	unsigned int w, h, border, depth;
	Display *dpy = ((t_xvar *)app->win->mlx)->display;
	Window win = ((t_win_list *)app->win->win)->window;
	XGetGeometry(dpy, win, &root, &x, &y, &w, &h, &border, &depth);

	if (app->win->width != (int)w || app->win->height != (int)h)
	{
		app->win->vtable->resize(app->win, w, h);
	}

	// Clear image buffer
	int bpp, size_line, endian;
	char *buf = app->win->vtable->get_image_buffer(app->win, &bpp, &size_line, &endian);
	if (buf)
		ft_memset(buf, 0, size_line * app->win->height);

	for (int i = 0; i < NUM_POINTS; ++i)
	{
		t_vec2 coord = app->camera->vtable->project_point(app->camera, app->points[i]);
		t_color col = app->points[i]->vtable->get_color(app->points[i]);
		int size = (int)(app->camera->zoom * 4.0);
		if (size < 1) size = 1;
		draw_filled_square_to_buffer(app->win, coord.x + app->win->draw_offset_x, coord.y + app->win->draw_offset_y, size, col.hex_color);
	}
	app->win->vtable->update_image(app->win);
	return (0);
}

// Add this function to handle window resize events
int on_window_resize(int new_width, int new_height, void *param)
{
	t_app *app = (t_app *)param;
	app->win->vtable->resize(app->win, new_width, new_height);
	redraw(app); // Force redraw after resizing
	return (0);
}

int main(void)
{
	t_app app;
	app.win = window_new(800, 600, "Camera Demo");
	if (!app.win)
		return (1);

	app.camera = camera_new(CAMERA_ISOMETRIC);
	if (!app.camera)
		return (1);

	t_input_handler *handler = InputHandler_new(app.camera);
	if (!handler)
		return (1);

	for (int i = 0; i < NUM_POINTS; ++i)
		app.points[i] = point_new_with_color(100 + i * 100, 100 + i * 50, i * 20, rgb_to_hex(50*i, 100, 200));

	input_handler_register(app.win, handler);
	mlx_loop_hook(app.win->mlx, redraw, &app);

	// Register the resize hook (pseudo-code, adapt to your MLX version)
	// mlx_hook(app.win->win, EVENT_WINDOW_RESIZE, 0, on_window_resize, &app);

	mlx_loop(app.win->mlx);

	for (int i = 0; i < NUM_POINTS; ++i)
		point_destroy(app.points[i]);
	camera_destroy(app.camera);
	app.win->vtable->destroy(app.win);
	return 0;
}
