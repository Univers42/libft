/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:01:23 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/29 21:12:20 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"
#include <stdlib.h>
#include "mlx.h"

void	window_init(t_window *self);
void	window_set_resizable(t_window *self);

void	window_close(t_window *self)
{
	if (!self)
		return ;
	self->vtable->destroy(self);
	exit(0);
}

const t_window_vtable	*get_window_vtable(void)
{
	static const t_window_vtable	vtable = {
		.destroy = window_destroy,
		.resize = window_resize,
		.init = window_init,
		.set_resizable = window_set_resizable,
		.close = window_close
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
