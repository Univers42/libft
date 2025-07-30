/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 17:19:30 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/30 03:41:52 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input_handler.h"
#include "window.h"
#include <unistd.h>
#include "mlx.h"

int main(void)
{
    t_window *win = window_new(800, 600, "Test Window with events");
    if (!win)
        return (1);

    t_input_handler *handler = InputHandler_new(NULL);
    if (!handler)
        return (1);

    win->vtable->set_resizable(win);
    input_handler_register(win, handler);

    mlx_loop(win->mlx);

    win->vtable->destroy(win);
    InputHandler_destroy(handler);
    return 0;
}