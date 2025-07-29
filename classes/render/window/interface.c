/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:01:23 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/29 19:39:25 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"
#include <stdlib.h>
#include "mlx.h"

// Forward declaration if not in header
void Window_init(t_Window *self);
void Window_set_resizable(t_Window *self);

void Window_close(t_Window *self)
{
	if (!self)
		return;
	self->vtable->destroy(self);
	exit(0);
}

const t_WindowVTable g_WindowVTable = {
	.destroy = Window_destroy,
	.resize = Window_resize,
	.init = Window_init,
	.set_resizable = Window_set_resizable,
	.close = Window_close
};

void Window_init(t_Window *self)
{
	if (!self)
		return;
	self->startw = START_W;
	self->starth = START_H;
	self->endh = END_H;
	self->steps = STEPS;
	self->endw = END_W;
}

t_Window *Window_new(int width, int height, const char *title)
{
	t_Window *win = malloc(sizeof(t_Window));
	if (!win)
		return (NULL);
	win->vtable = &g_WindowVTable;
	win->width = width;
	win->height = height;
	win->is_resizing = 0;
	win->mlx = mlx_init();
	win->win = mlx_new_window(win->mlx, width, height, (char *)(title ? title : "Window"));
	win->img = mlx_new_image(win->mlx, width, height);
	win->screen_buffer = malloc(sizeof(unsigned int) * width * height);
	if (win->screen_buffer)
		ft_memset(win->screen_buffer, 0, sizeof(unsigned int) * width * height);
	return win;
}

void Window_destroy(t_Window *self)
{
	if (!self)
		return;
	if (self->mlx && self->img)
		mlx_destroy_image(self->mlx, self->img);
	if (self->mlx && self->win)
		mlx_destroy_window(self->mlx, self->win);
	if (self->screen_buffer)
		free(self->screen_buffer);
	free(self);
}
