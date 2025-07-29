/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 10:59:22 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/29 20:51:44 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mlx.h"
#include "mlx_int.h"

#define MAX_KEYS 65536

// Add a key state array to track pressed keys
static char key_states[MAX_KEYS] = {0};

static void on_key_press(t_input_handler *self, t_window *win, int keycode)
{
	(void)self; (void)win;
	printf("[KEY PRESS] keycode=%d\n", keycode);
	if (keycode >= 0 && keycode < MAX_KEYS)
		key_states[keycode] = 1;
}

static void on_key_release(t_input_handler *self, t_window *win, int keycode)
{
	(void)self;
	printf("[KEY RELEASE] keycode=%d\n", keycode);
	if (keycode >= 0 && keycode < MAX_KEYS)
		key_states[keycode] = 0;
	if (keycode == 65307 && win && win->vtable && win->vtable->close)
		win->vtable->close(win);
}

static void on_mouse_press(t_input_handler *self, t_window *win, int button, int x, int y)
{
	(void)self; (void)win;
	printf("[MOUSE PRESS] button=%d x=%d y=%d\n", button, x, y);
}

static void on_mouse_release(t_input_handler *self, t_window *win, int button, int x, int y)
{
	(void)self; (void)win;
	printf("[MOUSE RELEASE] button=%d x=%d y=%d\n", button, x, y);
}

static void on_mouse_motion(t_input_handler *self, t_window *win, int x, int y)
{
	(void)self; (void)win;
	printf("[MOUSE MOTION] x=%d y=%d\n", x, y);
}

const t_input_handlerVTable g_InputHandlerVTable = {
	.on_key_press = on_key_press,
	.on_key_release = on_key_release,
	.on_mouse_press = on_mouse_press,
	.on_mouse_release = on_mouse_release,
	.on_mouse_motion = on_mouse_motion
};

t_input_handler *InputHandler_new(void)
{
	t_input_handler *handler = malloc(sizeof(t_input_handler));
	if (!handler)
		return NULL;
	handler->vtable = &g_InputHandlerVTable;
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
	(void)param;
	for (int k = 0; k < MAX_KEYS; ++k)
	{
		if (key_states[k])
			printf("[KEY HELD] keycode=%d\n", k);
	}
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