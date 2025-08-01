/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setsize.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 04:29:07 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/01 08:42:01 by dlesieur         ###   ########.fr       */
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

void	window_realloc_screen_buffer(t_window *window, int old_width, int old_height)
{
	if (!window)
		return ;
	unsigned int *old_buffer = (unsigned int *)window->screen_buffer;

	// Allocate new buffer first, don't free old_buffer yet
	unsigned int *new_buffer = malloc(sizeof(unsigned int) * window->width * window->height);
	if (!new_buffer)
	{
		// Allocation failed, keep old buffer
		return;
	}

	static unsigned int *bg_row = NULL;
	static int cached_width = 0;
	if (!bg_row || cached_width != window->width)
	{
		if (bg_row)
			free(bg_row);
		bg_row = malloc(sizeof(unsigned int) * window->width);
		cached_width = window->width;
	}
	if (bg_row)
		for (int i = 0; i < window->width; ++i)
			bg_row[i] = window->background_color;

	int y;
	for (y = 0; y < window->height; ++y)
	{
		if (old_buffer && y < old_height)
		{
			// Copy old row if possible
			int copy_width = (old_width < window->width) ? old_width : window->width;
			memcpy(&new_buffer[y * window->width], &old_buffer[y * old_width], sizeof(unsigned int) * copy_width);
			// Fill the rest with background color
			if (bg_row && copy_width < window->width)
				memcpy(&new_buffer[y * window->width + copy_width], bg_row, sizeof(unsigned int) * (window->width - copy_width));
		}
		else if (bg_row)
		{
			// Fill entire row with background color
			memcpy(&new_buffer[y * window->width], bg_row, sizeof(unsigned int) * window->width);
		}
	}

	if (old_buffer)
		free(old_buffer);

	window->screen_buffer = (char *)new_buffer;
}

// Poll the real window size and resize if needed
void	window_poll_resize(t_window *self)
{
	Window				win;
	Display				*display;
	XWindowAttributes	attr;
	static int			prev_width = -1;
	static int			prev_height = -1;

	if (!self || !self->mlx || !self->win)
		return ;
	if (self->is_resizing)
		return ;
	win = ((t_win_list *)self->win)->window;
	display = ((t_xvar *)self->mlx)->display;
	if (XGetWindowAttributes(display, win, &attr))
	{
		if (attr.width != self->width || attr.height != self->height)
		{
			// Only redraw if size actually changed since last poll
			if (attr.width != prev_width || attr.height != prev_height)
			{
				self->method->resize(self, attr.width, attr.height);
				prev_width = attr.width;
				prev_height = attr.height;
			}
		}
	}
}

bool	set_size(t_window *window, int width, int height)
{
	if (!window)
		return (false);
	int old_width = window->width;
	int old_height = window->height;
	window->width = width;
	window->height = height;
	window_realloc_screen_buffer(window, old_width, old_height);
	return (true);
}
