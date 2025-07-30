/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setsize.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 04:29:07 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/30 04:47:27 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"

// Call this when the user starts resizing
void	window_start_resizing(t_window *self)
{
	if (!self)
		return ;
	self->is_resizing = 1;
}

void	window_realloc_screen_buffer(t_window *window)
{
	if (!window)
		return ;
	if (window->screen_buffer)
		free(window->screen_buffer);
	window->screen_buffer = malloc(sizeof(unsigned int) * \
		window->width * window->height);
	if (window->screen_buffer)
		ft_memset(window->screen_buffer, 0, sizeof(unsigned int) * \
			window->width * window->height);
}

// Poll the real window size and resize if needed
void	window_poll_resize(t_window *self)
{
	Window				win;
	Display				*display;
	XWindowAttributes	attr;

	if (!self || !self->mlx || !self->win)
		return ;
	if (self->is_resizing)
		return ;
	win = ((t_win_list *)self->win)->window;
	display = ((t_xvar *)self->mlx)->display;
	if (XGetWindowAttributes(display, win, &attr))
		if (attr.width != self->width || attr.height != self->height)
			self->vtable->resize(self, attr.width, attr.height);
}

bool	set_size(t_window *window, int width, int height)
{
	if (!window)
		return (false);
	window->width = width;
	window->height = height;
	window_realloc_screen_buffer(window);
	return (true);
}
