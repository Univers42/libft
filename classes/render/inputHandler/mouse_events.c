/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_events.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 05:50:52 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/30 05:51:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input_handler.h"


static void on_mouse_press(t_input_handler *self, t_window *win, int button, int x, int y)
{
	(void)win;
	printf("[MOUSE PRESS] button=%d x=%d y=%d\n", button, x, y);
	// Left mouse button starts dragging
	if (self && self->camera && button == 1)
	{
		mouse_dragging = 1;
		last_mouse_x = x;
		last_mouse_y = y;
	}
	// Mouse wheel up (button 4): zoom in
	else if (self && self->camera && button == 4)
	{
		self->camera->vtable->zoom_by(self->camera, 1.1, x, y);
	}
	// Mouse wheel down (button 5): zoom out
	else if (self && self->camera && button == 5)
	{
		self->camera->vtable->zoom_by(self->camera, 0.9, x, y);
	}
}

static void on_mouse_release(t_input_handler *self, t_window *win, int button, int x, int y)
{
	(void)self; // Mark as unused to avoid warning
	(void)win;
	printf("[MOUSE RELEASE] button=%d x=%d y=%d\n", button, x, y);
	// Stop dragging on left mouse release
	if (button == 1)
		mouse_dragging = 0;
}

static void on_mouse_motion(t_input_handler *self, t_window *win, int x, int y)
{
	(void)win;
	printf("[MOUSE MOTION] x=%d y=%d\n", x, y);
	// If dragging, pan the camera
	if (self && self->camera && mouse_dragging)
	{
		int dx = x - last_mouse_x;
		int dy = y - last_mouse_y;
		self->camera->vtable->move(self->camera, dx, dy);
		last_mouse_x = x;
		last_mouse_y = y;
	}
}