/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:01:23 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/30 03:03:34 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"
#include <stdlib.h>

#include "input_handler.h"
#include "camera.h"

#define DRAW_OFFSET_X 300
#define DRAW_OFFSET_Y 200

void	window_init(t_window *self);
void	window_set_resizable(t_window *self);

void	window_close(t_window *self)
{
	if (!self)
		return ;
	self->vtable->destroy(self);
	exit(0);
}

/**
 * @brief for who's asking why we're using window as handler of
 * event like zooming when we have a camera. It's because 
 * the camera should be responsible for the zoom logic itself
 * (the math, state, effect on the view), but the window
 * (or event system) should be responsible for handling the mouse
 * wheel event and deciding when to call the camera's zoom function.
 * The camera knows how to zoom but should not know about events, mouse
 * or windowing
 * The camera only provide a clean API for zooming, pannign,etc..
 * @param self structure of window itself clalback
 * @param point x and y member
 * @param delta difference between point
 * @param ctrl type of ctrl
 * @return void just do the action no error expected
 */
void	window_handle_mouse_wheel(t_window *self, t_pos point,
			int delta, int ctrl)
{
	t_camera	*camera;
	double		factor;

	if (!self || !self->input_handler)
		return ;
	camera = input_handler_get_camera(self->input_handler);
	if (!camera)
		return ;
	if (ctrl)
	{
		if (delta > 0)
			factor = 1.1;
		else
			factor = 0.9;
		camera->vtable->zoom_by(camera, factor,
			point.x - self->draw_offset_x, point.y - self->draw_offset_y);
	}
}

t_window	*window_new(int width, int height, const char *title)
{
	t_window	*win;

	win = malloc(sizeof(t_window));
	if (!win)
		return (NULL);
	win->vtable = get_window_vtable();
	win->width = width;
	win->height = height;
	win->is_resizing = 0;
	win->mlx = mlx_init();
	if (title)
		win->win = mlx_new_window(win->mlx, width, height, (char *)(title));
	else
		win->win = mlx_new_window(win->mlx, width, height, "Window");
	win->img = mlx_new_image(win->mlx, width, height);
	win->screen_buffer = malloc(sizeof(unsigned int) * width * height);
	if (win->screen_buffer)
		ft_memset(win->screen_buffer, 0, sizeof(unsigned int) * width * height);
	win->draw_offset_x = 300;
	win->draw_offset_y = 200;
	return (win);
}

void	window_destroy(t_window *self)
{
	if (!self)
		return ;
	if (self->mlx && self->img)
		mlx_destroy_image(self->mlx, self->img);
	if (self->mlx && self->win)
		mlx_destroy_window(self->mlx, self->win);
	if (self->screen_buffer)
		free(self->screen_buffer);
	free(self);
}
