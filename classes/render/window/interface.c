/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:01:23 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/28 17:52:33 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"

//Constructor
t_Window *Window_new(t_server *server, int width, int height, const char *title)
{
	t_Window *win;

	if (!server)
		return (NULL);
	win = malloc(sizeof(t_Window));
	if (!win)
		return (NULL);
	win->vtable = &g_WindowVTable;
	win->server = server;
	win->width = width;
	win->height = height;
	win->is_resizing = 0;
	win->mlx = win->mlx;
	win->win = mlx_new_window(win->mlx, width, height, (char *) (title ? title : "Window"));
	win->img = NULL;
	win = win->win;
	win.vtable.resize(win, width, height);
	win->key_state.keycode = 0;
	return (win);
}

//Destructor
void    Window_destroy_impl(t_Window *self)
{
	if (!self)
		return ;
	if (self->server)
    {
		if (self->mlx && self->img)
			mlx_destroy_image(self->mlx, self->img);
		if (self->mlx && self->win)
			mlx_destroy_window(self->mlx, self->win);
	}
	free(self);
}

void Window_destroy(t_Window self)
{
	if (self && self.vtable && self.vtable.destroy())
		self.vtable.destroy(self);
}
