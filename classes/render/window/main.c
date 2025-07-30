/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 20:48:03 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/30 01:33:33 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"
#include "input_handler.h"
#include "mlx.h"
#include <unistd.h>

int	main(void)
{
	t_window		*win;
	t_input_handler	*handler;

	win = window_new(800, 600, "Test Window with events");
	if (!win)
		return (1);
	handler = InputHandler_new(NULL);
	if (!handler)
		return (1);
	win->vtable->set_resizable(win);
	input_handler_register(win, handler);
	mlx_loop(win->mlx);
	win->vtable->destroy(win);
	free(handler);
	return (0);
}
