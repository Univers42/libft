/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_events.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 05:50:01 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/30 05:50:38 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input_handler.h"

static void on_key_press(t_input_handler *self, t_window *win, int keycode)
{
	(void)win;
	printf("[KEY PRESS] keycode=%d\n", keycode);
	if (keycode >= 0 && keycode < MAX_KEYS)
		key_states[keycode] = 1;
	// Camera perspective switching
	if (self && self->camera)
	{
		// Perspective
		if (keycode == 't' || keycode == 'T')
			self->camera->vtable->set_perspective(self->camera, CAMERA_TOP);
		else if (keycode == 'i' || keycode == 'I')
			self->camera->vtable->set_perspective(self->camera, CAMERA_ISOMETRIC);
		else if (keycode == 's' || keycode == 'S')
			self->camera->vtable->set_perspective(self->camera, CAMERA_SIDE);

		// Arrow keys: update key_state struct for movement
		if (keycode == KEY_LEFT)
			key_state.left = 1;
		if (keycode == KEY_RIGHT)
			key_state.right = 1;
		if (keycode == KEY_UP)
			key_state.up = 1;
		if (keycode == KEY_DOWN)
			key_state.down = 1;

		// Move camera immediately on arrow key press for responsiveness
		if (keycode == KEY_LEFT)
			self->camera->vtable->move(self->camera, -10, 0);
		if (keycode == KEY_RIGHT)
			self->camera->vtable->move(self->camera, 10, 0);
		if (keycode == KEY_UP)
			self->camera->vtable->move(self->camera, 0, -10);
		if (keycode == KEY_DOWN)
			self->camera->vtable->move(self->camera, 0, 10);

		// Zoom ('+' and '-')
		if (keycode == 61 || keycode == 65451) // '+' (main and keypad)
			self->camera->vtable->zoom_by(self->camera, 1.1, win->width / 2, win->height / 2);
		if (keycode == 45 || keycode == 65453) // '-' (main and keypad)
			self->camera->vtable->zoom_by(self->camera, 0.9, win->width / 2, win->height / 2);
	}

	// Minimal confirmation: press 'r' to simulate resize start, 'e' to simulate resize end
	if (keycode == 'r' || keycode == 'R')
		window_start_resizing(win);
	if (keycode == 'e' || keycode == 'E')
		window_stop_resizing(win, win->width, win->height);
}

static void on_key_release(t_input_handler *self, t_window *win, int keycode)
{
	(void)self;
	printf("[KEY RELEASE] keycode=%d\n", keycode);
	if (keycode >= 0 && keycode < MAX_KEYS)
		key_states[keycode] = 0;

	// Arrow keys: update key_state struct for movement
	if (keycode == KEY_LEFT)
		key_state.left = 0;
	if (keycode == KEY_RIGHT)
		key_state.right = 0;
	if (keycode == KEY_UP)
		key_state.up = 0;
	if (keycode == KEY_DOWN)
		key_state.down = 0;

	if (keycode == 65307 && win && win->vtable && win->vtable->close)
		win->vtable->close(win);
}