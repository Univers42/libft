/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:28:50 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/08 15:32:47 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input_handler.h"
#include <mlx.h>
#include <stdlib.h>
#include <stdio.h>

t_input_event	*create_event(void)
{
	t_input_event	*e;
	int				i;

	e = (t_input_event *)malloc(sizeof(t_input_event));
	if (e == NULL)
		return (NULL);
	e->event_count = 0;
	e->mlx_ptr = NULL;
	e->win_ptr = NULL;
	e->add_event = input_add_event;
	e->prog_hook_events = input_prog_hook_events;
	e->destroy = input_destroy;
	i = 0;
	while (i < MAX_EVENTS)
	{
		e->events[i].keycode = 0;
		e->events[i].callback = NULL;
		e->events[i].param = NULL;
		i++;
	}
	return (e);
}

void	prog_hook_events_method(t_input_event *self, void *win)
{
	if (!self || !win)
		return ;
	self->win_ptr = win;
	self->mlx_ptr = NULL;
	printf("🔗 Setting up event hooks...\n");
	printf("   win_ptr=%p\n", win);
	printf("🎯 Hooking keyboard events...\n");
	mlx_key_hook(win, handle_keypress, self);
	printf("🎯 Hooking close events...\n");
	mlx_hook(win, 17, 1L << 17, handle_close, self);
	printf("✅ Event hooks established successfully\n");
}
