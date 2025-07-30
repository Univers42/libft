/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:01:16 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/30 03:22:49 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"
#include <stdlib.h>
#include <string.h>
#include "ft_stdlib.h"
#include "mlx.h"
#include "mlx_int.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "camera.h"
#include "input_handler.h"

static void window_realloc_screen_buffer(t_window *self)
{
	if (!self)
		return;
	if (self->screen_buffer)
		free(self->screen_buffer);
	self->screen_buffer = malloc(sizeof(unsigned int) * self->width * self->height);
	if (self->screen_buffer)
		ft_memset(self->screen_buffer, 0, sizeof(unsigned int) * self->width * self->height);
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

// Call this when the user starts resizing (e.g., on mouse button down on window border)
void window_start_resizing(t_window *self)
{
	if (!self) return;
	self->is_resizing = 1;
	printf("[DEBUG] window_start_resizing: is_resizing=1\n");
}

// Call this when the user releases the mouse button after resizing
// Forward declaration for redraw
extern int redraw(void *param);
extern void *g_app_ptr; // Use the global pointer set in main

void window_stop_resizing(t_window *self, int width, int height)
{
	if (!self) return;
	self->is_resizing = 0;
	printf("[DEBUG] window_stop_resizing: is_resizing=0\n");
	self->width = width;
	self->height = height;
	if (self->img)
		mlx_destroy_image(self->mlx, self->img);
	self->img = mlx_new_image(self->mlx, width, height);
	window_realloc_screen_buffer(self);

	// Redraw once after resizing ends
	if (g_app_ptr)
		redraw(g_app_ptr);
}

// Poll the real window size and resize if needed
void window_poll_resize(t_window *self)
{
	if (!self || !self->mlx || !self->win)
		return;
	if (self->is_resizing) {
		printf("[DEBUG] window_poll_resize: blocked, is_resizing=1\n");
		return;
	}

	Window win = ((t_win_list *)self->win)->window;
	Display *display = ((t_xvar *)self->mlx)->display;
	XWindowAttributes attr;
	if (XGetWindowAttributes(display, win, &attr))
	{
		if (attr.width != self->width || attr.height != self->height)
		{
			self->vtable->resize(self, attr.width, attr.height);
		}
	}
}
