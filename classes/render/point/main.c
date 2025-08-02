#include "point.h"
#include "window.h"
#include "input_handler.h"
#include "camera.h"
#include <unistd.h>
#include <string.h>
#include "mlx.h"

// Helper to draw a filled square centered at (x, y) with given size into the image buffer
static void draw_filled_square_to_buffer(t_window *win, int x, int y, int size, uint32_t color)
{
	int bpp = win->bpp, size_line = win->size_line;
	char *buf = win->img_data;
	if (!buf)
		return;
	int half = size / 2;
	for (int dy = -half; dy <= half; ++dy)
	{
		int py = y + dy;
		if (py < 0 || py >= win->height)
			continue;
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
	window_resize(win, new_width, new_height);
	return (0);
}

// Remove static/global variables and use a struct context allocated on the heap
typedef struct s_redraw_ctx
{
	t_window *win;
	t_camera *camera;
	t_point **points;
} t_redraw_ctx;

static int redraw(void *param)
{
	t_redraw_ctx *ctx = (t_redraw_ctx *)param;
	int size_line = ctx->win->size_line;
	char *buf = ctx->win->img_data;

	// If window size changed, resize buffer and redraw (skip X11 polling for now)
	// If you want to poll X11 size, add the code back and use win->mlx_ptr/win->win_ptr

	if (buf)
		memset(buf, 0, size_line * ctx->win->height);

	for (int i = 0; i < NUM_POINTS; ++i)
	{
		t_vec2 coord;
		t_color col;
		// Assign coord using project_point
		coord = ctx->camera->vtable->project_point(ctx->camera, ctx->points[i]);
		ctx->points[i]->vtable->get_color(ctx->points[i], &col);
		int size = (int)(ctx->camera->zoom * 4.0);
		if (size < 1)
			size = 1;
		draw_filled_square_to_buffer(ctx->win, coord.x, coord.y, size, col.hex_color);
	}
	window_render(ctx->win);
	return (0);
}

int main(void)
{
	t_window *win = NULL;
	t_camera *camera = NULL;
	t_input_handler *handler = NULL;
	static t_point *points[NUM_POINTS];
	int i, x, y;
	uint32_t color;

	win = window_new(800, 600, "Point Demo", 0x000000);
	if (!win)
		return (1);
	camera = camera_new(CAMERA_ISOMETRIC);
	if (!camera)
		return (1);
	handler = input_handler_new(camera);
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

	mlx_loop_hook(win->mlx_ptr, redraw, ctx);

	mlx_loop(win->mlx_ptr);

	for (i = 0; i < NUM_POINTS; ++i)
		point_destroy(points[i]);
	camera_destroy(camera);
	input_handler_destroy(handler);
	window_destroy(win);
	free(ctx);
	return (0);
}