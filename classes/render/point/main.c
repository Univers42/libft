#include "point.h"
#include "window.h"
#include "input_handler.h"
#include "camera.h"
#include <unistd.h>
#include "mlx.h" // Add this include for mlx_pixel_put and mlx_loop

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
			if (px < 0 || px >= win->width)
				continue;
			int offset = py * size_line + px * (bpp / 8);
			*(uint32_t *)(buf + offset) = color;
		}
	}
}

// Add this function to handle window resize events and buffer recreation
int on_window_resize(int new_width, int new_height, void *param)
{
	t_window *win = (t_window *)param;
	if (!win)
		return (0);
	win->vtable->resize(win, new_width, new_height);
	return (0);
}

// Remove static/global variables and use a struct context allocated on the heap
typedef struct s_redraw_ctx {
	t_window *win;
	t_camera *camera;
	t_point **points;
} t_redraw_ctx;

static int redraw(void *param)
{
	t_redraw_ctx *ctx = (t_redraw_ctx *)param;
	int bpp, size_line, endian;
	char *buf;

	// Poll actual window size
	Window root;
	int x, y;
	unsigned int w, h, border, depth;
	Display *dpy = ((t_xvar *)ctx->win->mlx)->display;
	Window win = ((t_win_list *)ctx->win->win)->window;
	XGetGeometry(dpy, win, &root, &x, &y, &w, &h, &border, &depth);

	// If window size changed, resize buffer and redraw
	if (ctx->win->width != (int)w || ctx->win->height != (int)h)
	{
		ctx->win->vtable->resize(ctx->win, w, h);
	}

	buf = ctx->win->vtable->get_image_buffer(ctx->win, &bpp, &size_line, &endian);
	if (buf)
		ft_memset(buf, 0, size_line * ctx->win->height);

	for (int i = 0; i < NUM_POINTS; ++i)
	{
		t_vec2 coord = ctx->camera->vtable->project_point(ctx->camera, ctx->points[i]);
		t_color col = ctx->points[i]->vtable->get_color(ctx->points[i]);
		int size = (int)(ctx->camera->zoom * 4.0);
		if (size < 1) size = 1;
		draw_filled_square_to_buffer(ctx->win, coord.x, coord.y, size, col.hex_color);
	}
	ctx->win->vtable->update_image(ctx->win);
	return (0);
}

int main(void)
{
	t_window		*win = NULL;
	t_camera		*camera = NULL;
	t_input_handler	*handler = NULL;
	static t_point	*points[NUM_POINTS];
	int i, x, y;
	uint32_t color;

	win = window_new(800, 600, "Point Demo");
	if (!win)
		return (1);
	camera = camera_new(CAMERA_ISOMETRIC);
	if (!camera)
		return (1);
	handler = InputHandler_new(camera);
	if (!handler)
		return (1);

	for (i = 0; i < NUM_POINTS; ++i)
	{
		x = 50 + i * 70;
		y = 100 + (i % 2) * 200;
		color = rgb_to_hex(20 * i, 255 - 20 * i, 100 + 10 * i);
		points[i] = point_new_with_color(x, y, 0, color);
	}

	input_handler_register(win, handler);

	// Allocate context on the heap to ensure it lives for the duration of the loop
	t_redraw_ctx *ctx = malloc(sizeof(t_redraw_ctx));
	ctx->win = win;
	ctx->camera = camera;
	ctx->points = points;

	mlx_loop_hook(win->mlx, redraw, ctx);

	mlx_loop(win->mlx);

	for (i = 0; i < NUM_POINTS; ++i)
		point_destroy(points[i]);
	camera_destroy(camera);
	win->vtable->destroy(win);
	free(ctx);
	return (0);
}