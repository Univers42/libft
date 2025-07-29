/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 17:19:30 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/29 19:40:34 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input_handler.h"
#include "window.h"
#include <unistd.h>
#include "mlx.h"

int main(void)
{
    t_Window *win = Window_new(800, 600, "Test Window with events");
    if (!win)
        return (1);

    t_InputHandler *handler = InputHandler_new();
    if (!handler)
        return (1);

    win->vtable->set_resizable(win);
    input_handler_register(win, handler);

    mlx_loop(win->mlx);

    win->vtable->destroy(win);
    free(handler);
    return 0;
}