/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:01:16 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/29 21:06:42 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"
#include <stdlib.h>
#include <string.h>
#include "mlx.h"
#include "mlx_int.h"
#include <X11/Xlib.h>

void	window_resize(t_window *self, int width, int height)
{
	if (!self)
		return ;
	self->width = width;
	self->height = height;
	XResizeWindow(((t_xvar *)self->mlx)->display,
		((t_win_list *)self->win)->window, width, height);
	if (self->img)
		mlx_destroy_image(self->mlx, self->img);
	self->img = mlx_new_image(self->mlx, width, height);
	if (self->screen_buffer)
		free(self->screen_buffer);
	self->screen_buffer = malloc(sizeof(unsigned int) * width * height);
	if (self->screen_buffer)
		memset(self->screen_buffer, 0, sizeof(unsigned int) * width * height);
}

void	window_set_resizable(t_window *self)
{
	XSizeHints	hints;

	if (!self)
		return ;
	hints.flags = PMinSize | PMaxSize | PSize | PPosition;
	hints.min_width = 1;
	hints.min_height = 1;
	hints.max_width = 10000;
	hints.max_height = 10000;
	hints.width = self->width;
	hints.height = self->height;
	hints.x = 0;
	hints.y = 0;
	XSetWMNormalHints(((t_xvar *)self->mlx)->display,
		((t_win_list *)self->win)->window, &hints);
}
