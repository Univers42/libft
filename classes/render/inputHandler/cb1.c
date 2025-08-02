/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cb1.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 05:48:38 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/30 08:25:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input_handler.h"

int	mlx_mouse_press_cb(int button, int x, int y, void *param)
{
	t_window		*win;
	t_input_handler	*handler;

	win = (t_window *)param;
	handler = (t_input_handler *)win->input_handler;
	if (handler && handler->vtable->on_mouse_press)
		handler->vtable->on_mouse_press(handler, win, button, x, y);
	return (0);
}

int	mlx_mouse_release_cb(int button, int x, int y, void *param)
{
	t_window		*win;
	t_input_handler	*handler;

	win = (t_window *)param;
	handler = (t_input_handler *)win->input_handler;
	if (handler && handler->vtable->on_mouse_release)
		handler->vtable->on_mouse_release(handler, win, button, x, y);
	return (0);
}

int	mlx_mouse_motion_cb(int x, int y, void *param)
{
	t_window		*win;
	t_input_handler	*handler;

	win = (t_window *)param;
	handler = (t_input_handler *)win->input_handler;
	if (handler && handler->vtable->on_mouse_motion)
		handler->vtable->on_mouse_motion(handler, win, x, y);
	return (0);
}
