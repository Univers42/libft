/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 10:59:22 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/02 18:47:13 by dlesieur         ###   ########.fr       */
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
#include <string.h> // Add this include for memset

// Remove the global variable and use a singleton accessor for key_state
t_key_state *get_key_state(void)
{
	static t_key_state key_state = {0, 0, 0, 0};

	return (&key_state);
}

static void compute_direction_vector(t_vectorN *dvec, t_key_state *key_state)
{
	// Use standard memset instead of ft_memset
	memset(dvec, 0, sizeof(t_vectorN));
	if (key_state->up)
		dvec->y -= 1;
	if (key_state->down)
		dvec->y += 1;
	if (key_state->left)
		dvec->x -= 1;
	if (key_state->right)
		dvec->x += 1;
}

// Add these externs or pass them via context as needed
extern t_point **g_points;
extern int g_point_count;

// In your movement update logic (call every frame or on key event):
void update_movement(t_camera *camera)
{
	double len;
	double speed;
	t_vectorN dvec;
	t_key_state *key_state;

	key_state = get_key_state();
	compute_direction_vector(&dvec, key_state);
	if (dvec.x != 0 || dvec.y != 0)
	{
		len = ft_sqrt(dvec.x * dvec.x + dvec.y * dvec.y);
		speed = 200.0;
		if (len > 0)
		{
			dvec.x = (dvec.x / len) * speed;
			dvec.y = (dvec.y / len) * speed;
		}
		// Move the camera (for view movement)
		if (camera)
			camera->vtable->move(camera, dvec.x, dvec.y);

		// Move all points (for object movement, if desired)
		// Uncomment if you want points to move instead of camera
		/*
		for (int i = 0; i < g_point_count; ++i)
			if (g_points[i])
				point_translate(g_points[i], dvec.x, dvec.y);
		*/
	}
}

// Implementation for input_handler_get_camera
t_camera *input_handler_get_camera(t_input_handler *handler)
{
	if (!handler)
		return (NULL);
	return (handler->camera);
}
