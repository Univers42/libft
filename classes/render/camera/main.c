#include "window.h"
#include "point.h"
#include "camera.h"
#include "input_handler.h"
#include <unistd.h>
#include "mlx.h"

#define NUM_POINTS 5

typedef struct s_app
{
	t_window *win;
	t_camera *camera;
	t_point *points[NUM_POINTS];
} t_app;

static int redraw(void *param)
{
	t_app *app = (t_app *)param;
	mlx_clear_window(app->win->mlx, app->win->win);
	for (int i = 0; i < NUM_POINTS; ++i)
	{
		t_vec2 coord = app->camera->vtable->project_point(app->camera, app->points[i]);
		t_color col = app->points[i]->vtable->get_color(app->points[i]);
		mlx_pixel_put(app->win->mlx, app->win->win, coord.x + 300, coord.y + 200, col.hex_color);
	}
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
	mlx_loop(app.win->mlx);

	for (int i = 0; i < NUM_POINTS; ++i)
		point_destroy(app.points[i]);
	camera_destroy(app.camera);
	app.win->vtable->destroy(app.win);
	return 0;
}
