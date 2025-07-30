/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 20:48:03 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/30 15:55:57 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"
#include "input_handler.h"
#include "mlx.h"
#include "line.h"
#include <unistd.h>

int	main(void)
{
	t_window		*win;
	t_input_handler	*handler;
	t_point			*p1;
	t_point			*p2;
	t_line			*line;

	win = window_new(800, 600, "Test Window with events");
	if (!win)
		return (1);
	handler = input_handler_new(NULL);
	if (!handler)
		return (1);
	win->vtable->set_resizable(win);
	input_handler_register(win, handler);
	// Example: draw a line into the window's image buffer
	p1 = point_new_with_color(100, 100, 0, 0xFF0000FF);
	p2 = point_new_with_color(700, 500, 0, 0x00FF00FF);
	line = line_new(p1, p2);
	window_draw_line(win, line);
	win->vtable->update_image(win);
	line->vtable->destroy(line);
	point_destroy(p1);
	point_destroy(p2);
	mlx_loop(win->mlx);
	win->vtable->destroy(win);
	free(handler);
	return (0);
}
