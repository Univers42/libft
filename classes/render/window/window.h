/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:01:09 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/28 17:54:50 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WINDOW_H
#define WINDOW_H

#include "fdf.h"

typedef struct s_Window t_Window;

typedef struct s_WindowVTable
{
	void (*destroy)			(t_Window *self);
	void (*resize)			(t_Window *self, int width, int height);
	void (*handle_key)		(t_Window *self, int keycode);
	void (*handle_mouse)	(t_Window *self, int button, int x, int y, int action);
	void (*redraw)			(t_Window *self);
}							t_WindowVTable;

typedef struct s_WindowKeyState
{
	int	keycode;
	int	is_pressed;
}		t_WindowKeyState;

struct s_Window
{
	const t_WindowVTable	*vtable;
	char					*screen_buffer;
	int						width;
	int						height;
	int						is_resizing;
	void					*mlx;
	void					*win;
	void					*img;
};

t_WindowVTable g_WindowVTable =
{
		.destroy = Window_destroy_impl,
		.resize = Window_resize_impl,
		.redraw = Window_redraw_impl,
};

t_Window	*Window_new(t_server *server, int width, int height, const char *title);
void		Window_destroy(t_Window *self);

#endif
