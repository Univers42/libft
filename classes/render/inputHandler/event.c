/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 10:59:22 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/30 01:13:59 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input_handler.h"
#include "camera.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mlx.h"
#include "mlx_int.h"
#include "ft_math.h"

#define MAX_KEYS 65536

// Add a key state array to track pressed keys
static char key_states[MAX_KEYS] = {0};

// Static variables for mouse drag
static int mouse_dragging = 0;
static int last_mouse_x = 0;
static int last_mouse_y = 0;

// Example key state struct (add to your input handler if not present)
typedef struct s_key_state {
	int up;
	int down;
	int left;
	int right;
} t_key_state;

// In your input handler struct:
static t_key_state key_state = {0, 0, 0, 0};

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

// In your movement update logic (call every frame or on key event):
void update_movement(t_camera *camera)
{
	double dx = 0, dy = 0;
	if (key_state.up)
		dy -= 1;
	if (key_state.down)
		dy += 1;
	if (key_state.left)
		dx -= 1;
	if (key_state.right)
		dx += 1;
	if (dx != 0 || dy != 0)
	{
		double len = ft_sqrt(dx * dx + dy * dy);
		double speed = 200.0; // Increase speed for faster movement
		if (len > 0)
		{
			dx = (dx / len) * speed;
			dy = (dy / len) * speed;
		}
		camera->vtable->move(camera, dx, dy);
	}
}

const t_input_handlerVTable g_InputHandlerVTable = {
	.on_key_press = on_key_press,
	.on_key_release = on_key_release,
	.on_mouse_press = on_mouse_press,
	.on_mouse_release = on_mouse_release,
	.on_mouse_motion = on_mouse_motion
};

t_input_handler *InputHandler_new(t_camera *camera)
{
	t_input_handler *handler = malloc(sizeof(t_input_handler));
	if (!handler)
		return NULL;
	handler->vtable = &g_InputHandlerVTable;
	handler->camera = camera; // Store camera pointer
	return handler;
}

static int mlx_key_press_cb(int keycode, void *param)
{
	t_window *win = (t_window *)param;
	t_input_handler *handler = (t_input_handler *)win->input_handler;
	if (handler && handler->vtable->on_key_press)
		handler->vtable->on_key_press(handler, win, keycode);
	return 0;
}

static int mlx_key_release_cb(int keycode, void *param)
{
	t_window *win = (t_window *)param;
	t_input_handler *handler = (t_input_handler *)win->input_handler;
	if (handler && handler->vtable->on_key_release)
		handler->vtable->on_key_release(handler, win, keycode);
	return 0;
}

static int mlx_mouse_press_cb(int button, int x, int y, void *param)
{
	t_window *win = (t_window *)param;
	t_input_handler *handler = (t_input_handler *)win->input_handler;
	if (handler && handler->vtable->on_mouse_press)
		handler->vtable->on_mouse_press(handler, win, button, x, y);
	return 0;
}

static int mlx_mouse_release_cb(int button, int x, int y, void *param)
{
	t_window *win = (t_window *)param;
	t_input_handler *handler = (t_input_handler *)win->input_handler;
	if (handler && handler->vtable->on_mouse_release)
		handler->vtable->on_mouse_release(handler, win, button, x, y);
	return 0;
}

static int mlx_mouse_motion_cb(int x, int y, void *param)
{
	t_window *win = (t_window *)param;
	t_input_handler *handler = (t_input_handler *)win->input_handler;
	if (handler && handler->vtable->on_mouse_motion)
		handler->vtable->on_mouse_motion(handler, win, x, y);
	return 0;
}

static int mlx_destroy_notify_cb(void *param)
{
	t_window *win = (t_window *)param;
	if (win && win->vtable && win->vtable->close)
		win->vtable->close(win);
	return 0;
}

// Loop hook for repeated key action
static int key_repeat_loop(void *param)
{
	t_window *win = (t_window *)param;
	t_input_handler *handler = (t_input_handler *)win->input_handler;
	if (handler && handler->camera)
		update_movement(handler->camera);
	return 0;
}

void input_handler_register(t_window *win, t_input_handler *handler)
{
	win->input_handler = handler;
	mlx_hook(win->win, 2, 1L<<0, mlx_key_press_cb, win); // KeyPress
	mlx_hook(win->win, 3, 1L<<1, mlx_key_release_cb, win); // KeyRelease
	mlx_mouse_hook(win->win, mlx_mouse_press_cb, win);
	mlx_hook(win->win, 5, 1L<<3, mlx_mouse_release_cb, win); // ButtonRelease
	mlx_hook(win->win, 6, 1L<<6, mlx_mouse_motion_cb, win); // MotionNotify
	mlx_hook(win->win, 17, 0, mlx_destroy_notify_cb, win); // DestroyNotify (window close)
	mlx_loop_hook(win->mlx, key_repeat_loop, win); // Add loop hook for key repeat
}

// Move these keycode definitions to the very top of the file, before any usage:
#ifndef KEY_UP
# define KEY_UP     65362 // Up arrow key (X11)
#endif
#ifndef KEY_DOWN
# define KEY_DOWN   65364 // Down arrow key (X11)
#endif
#ifndef KEY_LEFT
# define KEY_LEFT   65361 // Left arrow key (X11)
#endif
#ifndef KEY_RIGHT
# define KEY_RIGHT  65363 // Right arrow key (X11)
#endif