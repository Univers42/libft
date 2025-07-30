/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 05:54:45 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/30 05:55:06 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input_handler.h"

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

// Loop hook for repeated key action
static int key_repeat_loop(void *param)
{
	t_window		*win;
	t_input_handler	*handler;
	
	win = (t_window *)param;
	handler = (t_input_handler *)win->input->handler;
	if (handler && handler->camera)
		update_movement(handler->camera);
	return (0);
}
