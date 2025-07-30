/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 17:41:09 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/30 04:47:22 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"

void	window_realloc_screen_buffer(t_window *window);

void	window_update_image(t_window *self)
{
	if (!self || !self->img || !self->win)
		return ;
	mlx_put_image_to_window(self->mlx, self->win, self->img, 0, 0);
}

void	window_stop_resizing(t_window *self, int width, int height)
{
	if (!self)
		return ;
	self->is_resizing = 0;
	self->width = width;
	self->height = height;
	if (self->img)
		mlx_destroy_image(self->mlx, self->img);
	self->img = mlx_new_image(self->mlx, width, height);
	window_realloc_screen_buffer(self);
	if (self->redraw_cb)
		self->redraw_cb(self->redraw_ctx);
}

void	window_resize(t_window *self, int width, int height)
{
	if (!self)
		return ;
	self->width = width;
	self->height = height;
	if (self->img)
		mlx_destroy_image(self->mlx, self->img);
	self->img = mlx_new_image(self->mlx, width, height);
	window_realloc_screen_buffer(self);
}
