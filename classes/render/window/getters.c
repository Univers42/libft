/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:01:16 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/28 17:56:20 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"

void Window_resize(t_Window *self, int width, int height)
{
	if (!self || !self->server)
		return ;
	self->width = width;
	self->height = height;
	if (self->img)
		mlx_destroy_image(self->mlx, self->img);
	self->img = mlx_new_image(self->mlx, width, height);
	if (self->screen_buffer)
		free(self->screen_buffer);
	self->screen_buffer = malloc(sizeof(unsigned int) * width * height);
	memset(self->screen_buffer, 0, sizeof(unsigned int) * width * height);
}

// Redraw the image buffer to the window
void image_put(t_Window *self)
{
	if (!self)
		return ;
	mlx_put_image_to_window(self->mlx, self->win, self->img, 0, 0);
}
