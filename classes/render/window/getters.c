/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:01:16 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/30 04:48:47 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"

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

const t_window_vtable	*get_window_vtable(void)
{
	static const t_window_vtable	vtable = {
		.destroy = window_destroy,
		.resize = window_resize,
		.init = window_init,
		.set_resizable = window_set_resizable,
		.close = window_close,
		.get_image_buffer = window_get_image_buffer,
		.update_image = window_update_image
	};

	return (&vtable);
}

void	*window_get_image_buffer(t_window *self, int *bpp,
			int *size_line, int *endian)
{
	if (!self || !self->img)
		return (NULL);
	return (mlx_get_data_addr(self->img, bpp, size_line, endian));
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

// Forward declaration if not already present
void	window_resize(t_window *self, int width, int height);
