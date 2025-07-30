/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cb_key.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 05:51:18 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/30 05:51:58 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input_handler.h"


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

