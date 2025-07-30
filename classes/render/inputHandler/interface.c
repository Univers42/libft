/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 05:47:34 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/30 13:34:15 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input_handler.h"

void dispatch_event(t_input_handler *handler, t_window *win, int keycode, int is_press);

// Dummy implementations to satisfy linker (replace with real ones if available)
void on_key_press(t_input_handler *self, t_window *win, int keycode)
{
	if (self && win)
		dispatch_event(self, win, keycode, 1);
}

void on_key_release(t_input_handler *self, t_window *win, int keycode)
{
	if (self && win)
		dispatch_event(self, win, keycode, 0);
}

// Adapter for vtable: matches expected signature
static void on_mouse_release_adapter(t_input_handler *self, t_window *win, int button, int x, int y)
{
	(void)self;
	(void)win;
	(void)x;
	(void)y;
	on_mouse_release(button);
}

// Replace the global variable with a singleton accessor
const t_input_handlerVTable	*get_input_handler_vtable(void)
{
	static t_input_handlerVTable	vtable = {
		.on_key_press = on_key_press,
		.on_key_release = on_key_release,
		.on_mouse_press = on_mouse_press,
		.on_mouse_release = on_mouse_release_adapter,
		.on_mouse_motion = on_mouse_motion
	};

	return (&vtable);
}

t_input_handler	*input_handler_new(t_camera *camera)
{
	t_input_handler	*handler;

	handler = malloc(sizeof(t_input_handler));
	if (!handler)
		return (NULL);
	handler->vtable = get_input_handler_vtable();
	handler->camera = camera;
	return (handler);
}

int	mlx_destroy_notify_cb(void *param)
{
	t_window	*win;

	win = (t_window *)param;
	if (win && win->vtable && win->vtable->close)
		win->vtable->close(win);
	return (0);
}

void	input_handler_destroy(t_input_handler *handler)
{
	if (handler)
		free(handler);
}
