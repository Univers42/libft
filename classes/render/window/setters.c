/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 17:41:09 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/30 03:04:52 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"

static void window_realloc_screen_buffer(t_window *window);

bool	set_size(t_window *window, int width, int height)
{
	if (!window)
		return (false);
	window->width = width;
	window->height = height;
	window_realloc_screen_buffer(window);
	return (true);
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

void	window_update_image(t_window *self)
{
	if (!self || !self->img || !self->win)
		return ;
	mlx_put_image_to_window(self->mlx, self->win, self->img, 0, 0);
}

static void window_realloc_screen_buffer(t_window *window)
{
	if (!window)
		return;
	if (window->screen_buffer)
		free(window->screen_buffer);
	window->screen_buffer = malloc(sizeof(unsigned int) * window->width * window->height);
	if (window->screen_buffer)
		ft_memset(window->screen_buffer, 0, sizeof(unsigned int) * window->width * window->height);
}
