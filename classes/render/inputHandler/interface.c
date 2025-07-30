/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 05:47:34 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/30 05:49:23 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"input_handler.h"

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

int mlx_destroy_notify_cb(void *param)
{
	t_window *win;

	(t_window *)param;
	if (win && win->vtable && win->vtable->close)
		win->vtable->close(win);
	return (0);
}

void InputHandler_destroy(t_input_handler *handler)
{
	if (handler)
		free(handler);
}