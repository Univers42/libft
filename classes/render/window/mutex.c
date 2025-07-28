/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:01:16 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/28 11:01:17 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"

void Window_resize_impl(t_Window *self, int width, int height)
{
	if (!self || !self->server)
		return ;
	self->width = width;
	self->height = height;
	self->server->width = width;
	self->server->height = height;
	if (self->server->img)
		mlx_destroy_image(self->server->mlx, self->server->img);
	self->server->img = mlx_new_image(self->server->mlx, width, height);
	if (self->server->screen_buffer)
		free(self->server->screen_buffer);
	self->server->screen_buffer = malloc(sizeof(unsigned int) * width * height);
	memset(self->server->screen_buffer, 0, sizeof(unsigned int) * width * height);
}

void	Window_handle_mouse_impl(t_Window *self, int button, int x, int y, int action)
{
	if (!self)
		return;
	const char *action_str = (action == 1) ? "press" : (action == 0) ? "release" : (action == 2) ? "motion" : "other";
	printf("[Window] Mouse event: button=%d x=%d y=%d action=%s\n", button, x, y, action_str);
	// Example: start/stop resizing on left mouse button
	if (button == 1 && action == 1) { // Press
		self->is_resizing = 1;
	} else if (button == 1 && action == 0) { // Release
		self->is_resizing = 0;
	}
	// Optionally handle drag to resize
	if (self->is_resizing && action == 2) { // Motion
		// Clamp minimum size
		int minw = 200, minh = 200;
		int neww = (x > minw) ? x : minw;
		int newh = (y > minh) ? y : minh;
		self->vtable->resize(self, neww, newh);
	}
}

void Window_redraw_impl(t_Window *self)
{
	if (!self || !self->server)
		return;
	// Redraw the image buffer to the window
	mlx_put_image_to_window(self->server->mlx, self->server->win, self->server->img, 0, 0);
}
