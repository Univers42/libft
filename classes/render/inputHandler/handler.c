/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 05:54:45 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/02 18:47:18 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input_handler.h"

void input_handler_register(t_window *win, t_input_handler *handler)
{
	win->input_handler = handler;
	mlx_hook(win->win_ptr, 2, 1L << 0, mlx_key_press_cb, win);
	mlx_hook(win->win_ptr, 3, 1L << 1, mlx_key_release_cb, win);
	mlx_mouse_hook(win->win_ptr, mlx_mouse_press_cb, win);
	mlx_hook(win->win_ptr, 5, 1L << 3, mlx_mouse_release_cb, win);
	mlx_hook(win->win_ptr, 6, 1L << 6, mlx_mouse_motion_cb, win);
	mlx_hook(win->win_ptr, 17, 0, mlx_destroy_notify_cb, win);
	mlx_loop_hook(win->mlx_ptr, key_repeat_loop, win);
}

// Loop hook for repeated key action
int key_repeat_loop(void *param)
{
	t_window *win;
	t_input_handler *handler;

	win = (t_window *)param;
	handler = (t_input_handler *)win->input_handler;
	if (handler && handler->camera)
		update_movement(handler->camera);

	// If you have a redraw callback, call it here (otherwise, remove/comment out)
	// Example:
	// if (win->redraw_cb)
	//     win->redraw_cb(win->redraw_ctx);

	// If you have a cache system, update it here (otherwise, remove/comment out)
	// Example:
	// if (win->cache && win->cache->cache_enabled && !win->cache->background_cached)
	//     window_cache_render_background(win);
	// if (win->cache && win->cache->cache_enabled)
	//     window_cache_smart_update(win);

	return (0);
}
