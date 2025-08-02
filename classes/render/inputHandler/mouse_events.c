/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_events.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 05:50:52 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/30 13:53:29 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input_handler.h"

static int mouse_dragging = 0;
static int last_mouse_x = 0;
static int last_mouse_y = 0;

void on_mouse_press(t_input_handler *self, t_window *win, int button, int x, int y)
{
    (void)win;
    if (!self || !self->camera)
        return;
    if (button == 1) // Left mouse button: start dragging for pan
    {
        mouse_dragging = 1;
        last_mouse_x = x;
        last_mouse_y = y;
    }
    else if (button == 4) // Scroll up: zoom in
    {
        self->camera->vtable->zoom_by(self->camera, 1.1, x, y);
    }
    else if (button == 5) // Scroll down: zoom out
    {
        self->camera->vtable->zoom_by(self->camera, 0.9, x, y);
    }
    // Do NOT create or add points here!
}

void on_mouse_release(int button)
{
    if (button == 1)
        mouse_dragging = 0;
}

void on_mouse_motion(t_input_handler *self, t_window *win, int x, int y)
{
	(void)win;
    if (!self || !self->camera)
        return;
    if (mouse_dragging)
    {
        double dx = x - last_mouse_x;
        double dy = y - last_mouse_y;
        self->camera->vtable->move(self->camera, dx, dy);
        last_mouse_x = x;
        last_mouse_y = y;
    }
}