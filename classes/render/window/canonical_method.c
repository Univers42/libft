/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   canonical_method.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:01:23 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/28 11:01:24 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"

//Constructor
t_Window *Window_new(t_server *server, int width, int height, const char *title) {
	if (!server)
		return NULL;
	t_Window *win = malloc(sizeof(t_Window));
	if (!win)
		return NULL;
	win->vtable = &g_WindowVTable;
	win->server = server;
	win->width = width;
	win->height = height;
	win->is_resizing = 0;
	win->mlx = server->mlx;
	win->win = mlx_new_window(win->mlx, width, height, (char *) (title ? title : "Window"));
	win->img = NULL;
	server->win = win->win;
	// Create image and buffer
	win->vtable->resize(win, width, height);
	win->key_state.keycode = 0;
	win->key_state.is_pressed = 0;
	win->mouse_left_pressed = 0;
	win->mouse_right_pressed = 0;
	win->mouse_motion_active = 0;

	// Set up MLX hooks
	mlx_key_hook(win->win, mlx_key_hook_cb, win);
	mlx_hook(win->win, 3, 1L << 1, mlx_key_release_cb, win); // Key release
	mlx_mouse_hook(win->win, mlx_mouse_press_cb, win); // Button press
	mlx_hook(win->win, 5, 1L << 3, mlx_mouse_release_cb, win); // Button release
	mlx_hook(win->win, 6, 1L << 6, mlx_mouse_motion_cb, win); // MotionNotify
	mlx_loop_hook(win->mlx, mlx_loop_hook_cb, win); // Loop event

	return win;
}

//Destructor
void    Window_destroy_impl(t_Window *self)
{
	if (!self)
		return;
	if (self->server)
    {
		if (self->server->mlx && self->server->img)
			mlx_destroy_image(self->server->mlx, self->server->img);
		if (self->server->mlx && self->server->win)
			mlx_destroy_window(self->server->mlx, self->server->win);
	}
	free(self);
}