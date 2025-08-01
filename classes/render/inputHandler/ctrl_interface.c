/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctrl_interface.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 11:50:41 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/01 07:53:54 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input_handler.h"
#include "window.h"
#include <stdio.h>

// Example: Handle window interface events (resize, background color, etc.)
void handle_interface_key(t_input_handler *handler, t_window *win, int keycode)
{
    (void)handler;
    if (!win || !win->method)
        return;
    if (keycode == 'r')
    {
        // Example: Toggle window resizable
        printf("[INTERFACE] Key 'r' pressed: toggling resizable\n");
        win->method->set_resizable(win);
    }
    else if (keycode == 'b')
    {
        // Example: Change background color (dummy value)
        printf("[INTERFACE] Key 'b' pressed: changing background color\n");
        win->method->set_background_color(win, 0x003366);
    }
    else if (keycode == 'c')
    {
        // Example: Clear window
        printf("[INTERFACE] Key 'c' pressed: clearing window\n");
        win->method->clear(win);
    }
    // Add more interface events as needed
}
