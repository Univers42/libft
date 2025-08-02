/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verif.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 09:11:07 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/30 13:11:16 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input_handler.h"

// Return the event mask for a given keycode/button
int get_event_mask(int keycode)
{
	if (keycode == KEY_LEFT || keycode == KEY_RIGHT ||
		keycode == KEY_UP || keycode == KEY_DOWN)
		return (EVENT_MOVE);
	if (keycode == 't' || keycode == 'T' ||
		keycode == 'i' || keycode == 'I' ||
		keycode == 's' || keycode == 'S')
		return (EVENT_CAMERA);
	if (keycode == 61 || keycode == 65451 || // '=' or numpad '+'
		keycode == 45 || keycode == 65453)    // '-' or numpad '-'
		return (EVENT_ZOOM);
	if (keycode == 'r' || keycode == 'R' ||
		keycode == 'e' || keycode == 'E')
		return (EVENT_WINDOW);
	if (keycode == 65307) // Escape
		return (EVENT_EXIT);
	if (keycode == MOUSE_LEFT || keycode == MOUSE_MIDDLE ||
		keycode == MOUSE_RIGHT || keycode == MOUSE_SCROLL_UP ||
		keycode == MOUSE_SCROLL_DOWN)
		return EVENT_MOUSE;
	return (EVENT_NONE);
}

// General function to check if a keycode belongs to a category
int what_category(int keycode, t_event_category category)
{
	return (get_event_mask(keycode) & category);
}

// Static helpers for move and camera categories
static int is_move(int keycode) {
    return what_category(keycode, EVENT_MOVE);
}

static int is_camera(int keycode) {
    return what_category(keycode, EVENT_CAMERA);
}

// Specific helpers for each category
t_verif is_e_move(int keycode)
{
    (void)keycode;
    return is_move;
}

t_verif is_e_camera(int keycode)
{
    (void)keycode;
    return is_camera;
}


