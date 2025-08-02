/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 01:03:47 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/02 18:52:06 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line.h"
#include "window.h"
#include "input_handler.h"
#include "mlx.h"
#include <stdio.h>
#include <unistd.h>

// Adapter to match t_pixel_callback signature for window_put_pixel
static void window_pixel_callback(int x, int y, uint32_t color, void *data)
{
	t_window *win = (t_window *)data;
	window_put_pixel(win, x, y, color);
}

int main(void)
{
	t_window *win;
	t_input_handler *handler;
	t_point *p1;
	t_point *p2;
	t_point *p3;
	t_point *p4;
	t_line *line1;
	t_line *line2;
	t_line *line3;

	// Create window and input handler
	win = window_new(800, 600, "Line Demo in Window", 0x000000); // Add bg_color argument
	if (!win)
		return (1);
	handler = input_handler_new(NULL);
	if (!handler)
		return (1);
	// Remove win->method->set_resizable(win);

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
	for (int i = 0; i < 3; ++i)
	{
		t_line *line = (i == 0 ? line1 : (i == 1 ? line2 : line3));
		if (line && line->vtable && line->vtable->draw_bresenham)
		{
			line->vtable->draw_bresenham(line, window_pixel_callback, win);
		}
	}

	// Show the result
	window_render(win);

	// Cleanup lines and points (window does not own them)
	line1->vtable->destroy(line1);
	line2->vtable->destroy(line2);
	line3->vtable->destroy(line3);
	point_destroy(p1);
	point_destroy(p2);
	point_destroy(p3);
	point_destroy(p4);

	// Enter event loop
	mlx_loop(win->mlx_ptr);

	window_destroy(win);
	free(handler);
	return (0);
}