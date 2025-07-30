/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 10:59:22 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/30 05:55:30 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input_handler.h"
#include "camera.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mlx.h"
#include "mlx_int.h"
#include "ft_math.h"

#define MAX_KEYS 65536

// Add a key state array to track pressed keys
static char key_states[MAX_KEYS] = {0};

// Static variables for mouse drag
static int mouse_dragging = 0;
static int last_mouse_x = 0;
static int last_mouse_y = 0;

// Example key state struct (add to your input handler if not present)
typedef struct s_key_state {
	int up;
	int down;
	int left;
	int right;
} t_key_state;

// In your input handler struct:
static t_key_state key_state = {0, 0, 0, 0};




// In your movement update logic (call every frame or on key event):
void update_movement(t_camera *camera)
{
	double dx = 0, dy = 0;
	if (key_state.up)
		dy -= 1;
	if (key_state.down)
		dy += 1;
	if (key_state.left)
		dx -= 1;
	if (key_state.right)
		dx += 1;
	if (dx != 0 || dy != 0)
	{
		double len = ft_sqrt(dx * dx + dy * dy);
		double speed = 200.0; // Increase speed for faster movement
		if (len > 0)
		{
			dx = (dx / len) * speed;
			dy = (dy / len) * speed;
		}
		camera->vtable->move(camera, dx, dy);
	}
}











// Move these keycode definitions to the very top of the file, before any usage:
#ifndef KEY_UP
# define KEY_UP     65362 // Up arrow key (X11)
#endif
#ifndef KEY_DOWN
# define KEY_DOWN   65364 // Down arrow key (X11)
#endif
#ifndef KEY_LEFT
# define KEY_LEFT   65361 // Left arrow key (X11)
#endif
#ifndef KEY_RIGHT
# define KEY_RIGHT  65363 // Right arrow key (X11)
#endif

// Implementation for input_handler_get_camera
t_camera *input_handler_get_camera(t_input_handler *handler)
{
	if (!handler)
		return (NULL);
	return (handler->camera);
}