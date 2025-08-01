/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 01:03:47 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/30 15:57:22 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line.h"
#include "window.h"
#include "input_handler.h"
#include "mlx.h"
#include <stdio.h>
#include <unistd.h>

int	main(void)
{
	t_window		*win;
	t_input_handler	*handler;
	t_point			*p1;
	t_point			*p2;
	t_point			*p3;
	t_point			*p4;
	t_line			*line1;
	t_line			*line2;
	t_line			*line3;

	// Create window and input handler
	win = window_new(800, 600, "Line Demo in Window");
	if (!win)
		return (1);
	handler = input_handler_new(NULL);
	if (!handler)
		return (1);
	win->method->set_resizable(win); // <-- use .method instead of .vtable
	input_handler_register(win, handler);

	// Create points and lines
	p1 = point_new_with_color(100, 100, 0, 0xFF0000FF); // Red
	p2 = point_new_with_color(700, 500, 0, 0x00FF00FF); // Green
	p3 = point_new_with_color(400, 100, 0, 0xFFFF00FF); // Yellow
	p4 = point_new_with_color(400, 500, 0, 0x0000FFFF); // Blue

	line1 = line_new(p1, p2);
	line2 = line_new(p3, p4);
	line3 = line_new_with_colors(100, 500, 0, 0xFFFFFFFF, 700, 100, 0, 0xFF00FFFF);

	// Draw lines into window buffer
	window_draw_line(win, line1);
	window_draw_line(win, line2);
	window_draw_line(win, line3);

	// Show the result
	win->method->update_image(win); // <-- use .method instead of .vtable

	// Cleanup lines and points (window does not own them)
	line1->vtable->destroy(line1);
	line2->vtable->destroy(line2);
	line3->vtable->destroy(line3);
	point_destroy(p1);
	point_destroy(p2);
	point_destroy(p3);
	point_destroy(p4);

	// Enter event loop
	mlx_loop(win->mlx);

	win->method->destroy(win); // <-- use .method instead of .vtable
	free(handler);
	return (0);
}