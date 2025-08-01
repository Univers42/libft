/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 20:48:03 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/01 08:51:48 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"
#include "input_handler.h"
#include "mlx.h"
#include "line.h"
#include <unistd.h>

int main(void)
{
    t_window        *win;
    t_input_handler *handler;
    t_point         *p1;
    t_point         *p2;
    t_line          *line;

    win = window_new(800, 600, "Test Window with Cache");
    if (!win)
        return (1);
    
    handler = input_handler_new(NULL);
    if (!handler)
        return (1);
    
    win->method->set_resizable(win);
    input_handler_register(win, handler);
    
    // Set background color and render static background
    win->method->set_background_color(win, 0x001122FF);
    window_cache_render_background(win);
    
    // Example: draw a line (will go to cache layer automatically)
    p1 = point_new_with_color(100, 100, 0, 0xFF0000FF);
    p2 = point_new_with_color(700, 500, 0, 0x00FF00FF);
    line = line_new(p1, p2);
    
    window_draw_line(win, line);  // Automatically uses cache
    win->method->update_image(win);  // Smart cache update
    
    line->vtable->destroy(line);
    point_destroy(p1);
    point_destroy(p2);
    
    mlx_loop(win->mlx);
    win->method->destroy(win);
    free(handler);
    return (0);
}