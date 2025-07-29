/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 20:48:03 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/29 21:01:59 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"
#include <unistd.h>

int	main(void)
{
	t_window	*win;

	win = window_new(800, 600, "Test Window");
	if (!win)
		return (1);
	win->vtable->init(win);
	usleep(200000);
	win->vtable->set_resizable(win);
	usleep(1000000);
	win->vtable->set_resizable(win);
	while (1)
		sleep(1);
	win->vtable->destroy(win);
	return (0);
}
