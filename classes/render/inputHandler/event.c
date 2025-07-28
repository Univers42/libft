/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 10:59:22 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/28 17:39:34 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input_handler.h"

void Window_handle_key_event(t_Window *self, int keycode)
{
    if (self && self->vtable && self->vtable->handle_key)
        self->vtable->handle_key(self, keycode);
}

// Add a key release handler
static int mlx_key_release_cb(int keycode, void *param)
{
	t_Window *win = (t_Window *) param;
	if (win) {
		// Mark key as released
		if (win->key_state.keycode == keycode)
			win->key_state.is_pressed = 0;
	}
	return 0;
}

// Loop event handler
int mlx_loop_hook_cb(void *param) {
	t_Window *win = (t_Window *) param;
	// If no mouse motion is active, repeat key event if pressed
	if (win && win->key_state.is_pressed && !win->mouse_motion_active) {
		win->vtable->handle_key(win, win->key_state.keycode);
	}
	// Reset mouse_motion_active for next loop
	if (win)
		win->mouse_motion_active = 0;
	return 0;
}

void Window_handle_key_impl(t_Window *self, int keycode) {
	if (!self)
		return;
	printf("[Window] Key event: keycode=%d\n", keycode);
	if (keycode == 65307) // ESC
		exit(0);
	// Add more key handling as needed
	// Mark key as pressed
	self->key_state.keycode = keycode;
	self->key_state.is_pressed = 1;
}


// Mouse press callback
int mlx_mouse_press_cb(int button, int x, int y, void *param)
{
	t_Window *win;

    win = (t_Window*)param;
	if (button == 1)
		win->mouse_left_pressed = 1;
	else if (button == 3)
		win->mouse_right_pressed = 1;
	win->vtable->handle_mouse(win, button, x, y, 1);
	return (0);
}

// Mouse release callback
int mlx_mouse_release_cb(int button, int x, int y, void *param)
{
	t_Window *win;

    win = (t_Window*)param;
	if (button == 1)
		win->mouse_left_pressed = 0;
	else if (button == 3)
		win->mouse_right_pressed = 0;
	win->vtable->handle_mouse(win, button, x, y, 0);
	return (0);
}

// Mouse motion callbac
int mlx_mouse_motion_cb(int x, int y, void *param)
{
	t_Window *win;

    win = (t_Window*)param;
	// Only track/print mouse motion if left or right button is held
	if (win->mouse_left_pressed)
    {
		win->mouse_motion_active = 1;
		win->vtable->handle_mouse(win, 1, x, y, 2);
	}
    else if (win->mouse_right_pressed)
    {
		win->mouse_motion_active = 1;
		win->vtable->handle_mouse(win, 3, x, y, 2);
	}
	return (0);
}

int mlx_key_hook_cb(int keycode, void *param)
{
	t_Window *win;

	win = (t_Window *) param;
	// Use the new method for handling key events
	Window_handle_key_event(win, keycode);
	return (0);
}

void	Window_handle_mouse_impl(t_Window *self, int button, int x, int y, int action)
{
	if (!self)
		return;
	const char *action_str = (action == 1) ? "press" : (action == 0) ? "release" : (action == 2) ? "motion" : "other";
	printf("[Window] Mouse event: button=%d x=%d y=%d action=%s\n", button, x, y, action_str);
	// Example: start/stop resizing on left mouse button
	if (button == 1 && action == 1) { // Press
		self->is_resizing = 1;
	} else if (button == 1 && action == 0) { // Release
		self->is_resizing = 0;
	}
	// Optionally handle drag to resize
	if (self->is_resizing && action == 2) { // Motion
		// Clamp minimum size
		int minw = 200, minh = 200;
		int neww = (x > minw) ? x : minw;
		int newh = (y > minh) ? y : minh;
		self->vtable->resize(self, neww, newh);
	}
}
