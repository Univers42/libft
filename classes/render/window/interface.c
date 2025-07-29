/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:01:23 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/30 00:54:17 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"
#include <stdlib.h>
#include "mlx.h"
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

void* window_get_image_buffer(t_window *self, int *bpp, int *size_line, int *endian)
{
	if (!self || !self->img)
		return NULL;
	return mlx_get_data_addr(self->img, bpp, size_line, endian);
}

void window_update_image(t_window *self)
{
	if (!self || !self->img || !self->win)
		return;
	mlx_put_image_to_window(self->mlx, self->win, self->img, 0, 0);
}

void	window_handle_mouse_wheel(t_window *self, int x, int y, int delta, int ctrl)
{
	if (!self || !self->input_handler)
		return;
	t_camera *camera = input_handler_get_camera(self->input_handler);
	if (!camera)
		return;
	if (ctrl)
	{
		double factor = (delta > 0) ? 1.1 : 0.9;
		// Subtract the drawing offset to centralize zoom under the mouse
		camera->vtable->zoom_by(camera, factor, x - self->draw_offset_x, y - self->draw_offset_y);
	}
}

const t_window_vtable	*get_window_vtable(void)
{
	static const t_window_vtable	vtable = {
		.destroy = window_destroy,
		.resize = window_resize,
		.init = window_init,
		.set_resizable = window_set_resizable,
		.close = window_close,
		.get_image_buffer = window_get_image_buffer,
		.update_image = window_update_image,
		.handle_mouse_wheel = window_handle_mouse_wheel
	};

	return (&vtable);
}

void	window_init(t_window *self)
{
	if (!self)
		return ;
	self->startw = START_W;
	self->starth = START_H;
	self->endh = END_H;
	self->steps = STEPS;
	self->endw = END_W;
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
	win->draw_offset_x = 300; // default for camera demo
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
