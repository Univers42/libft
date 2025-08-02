/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctrl_interface.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 11:50:41 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/02 18:44:51 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input_handler.h"
#include "window.h"
#include <stdio.h>

// Example: Handle window interface events (resize, background color, etc.)
void handle_interface_key(t_input_handler *handler, t_window *win, int keycode)
{
    (void)handler;
    if (!win || !win->vtable)
        return;
    if (keycode == 'r')
    {
        // Example: Toggle window resizable (implement if you have this in your vtable)
        // If not implemented, you can ignore or add your own function.
        // printf("[INTERFACE] Key 'r' pressed: toggling resizable\n");
        // win->vtable->set_resizable(win);
    }
    else if (keycode == 'b')
    {
        // Example: Change background color (dummy value)
        printf("[INTERFACE] Key 'b' pressed: changing background color\n");
        window_set_background(win, 0x003366);
        window_clear(win);
        window_render(win);
    }
    else if (keycode == 'c')
    {
        // Example: Clear window
        printf("[INTERFACE] Key 'c' pressed: clearing window\n");
        window_clear(win);
        window_render(win);
    }
    // Add more interface events as needed
}
