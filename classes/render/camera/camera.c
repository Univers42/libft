/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:03:46 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/30 00:44:27 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "camera.h"
#include <stdlib.h>
#include <math.h>

static void camera_set_perspective(t_camera *self, t_camera_perspective perspective)
{
	if (self)
		self->perspective = perspective;
}

static t_camera_perspective camera_get_perspective(t_camera *self)
{
	if (!self)
		return CAMERA_ISOMETRIC;
	return self->perspective;
}

static void camera_move(t_camera *self, double dx, double dy)
{
	if (!self) return;
	self->offset_x += dx;
	self->offset_y += dy;
}

// centralized zoom: center_x, center_y are in screen coordinates
static void camera_set_zoom(t_camera *self, double zoom, double center_x, double center_y)
{
	if (!self) return;
	if (zoom < 0.1) zoom = 0.1;
	double old_zoom = self->zoom;
	// Keep the world position under (center_x, center_y) fixed
	self->offset_x = center_x - (center_x - self->offset_x) * (zoom / old_zoom);
	self->offset_y = center_y - (center_y - self->offset_y) * (zoom / old_zoom);
	self->zoom = zoom;
}

static void camera_zoom_by(t_camera *self, double factor, double center_x, double center_y)
{
	if (!self) return;
	double new_zoom = self->zoom * factor;
	if (new_zoom < 0.1) new_zoom = 0.1;
	camera_set_zoom(self, new_zoom, center_x, center_y);
}

static t_vec2 camera_project_point(t_camera *self, t_point *point)
{
	t_vec2 result = {0};
	if (!self || !point)
		return result;
	t_position pos = point->vtable->get_position(point);
	double x = pos.x, y = pos.y, z = pos.z;
	double px = 0, py = 0;

	if (self->perspective == CAMERA_ISOMETRIC)
	{
		px = (x - y);
		py = ((x + y) / 2 - z);
	}
	else if (self->perspective == CAMERA_TOP)
	{
		px = x;
		py = y;
	}
	else if (self->perspective == CAMERA_SIDE)
	{
		px = x;
		py = z;
	}
	// Apply zoom and pan
	result.x = (int)(px * self->zoom + self->offset_x);
	result.y = (int)(py * self->zoom + self->offset_y);
	return result;
}

static void camera_destroy_method(t_camera *self)
{
	if (self)
		free(self);
}

static t_camera_vtable g_camera_vtable = {
	.set_perspective = camera_set_perspective,
	.get_perspective = camera_get_perspective,
	.project_point = camera_project_point,
	.move = camera_move,
	.set_zoom = camera_set_zoom,      // updated signature
	.zoom_by = camera_zoom_by,        // updated signature
	.destroy = camera_destroy_method
};

t_camera *camera_new(t_camera_perspective perspective)
{
	t_camera *cam = malloc(sizeof(t_camera));
	if (!cam)
		return NULL;
	cam->perspective = perspective;
	cam->offset_x = 0;
	cam->offset_y = 0;
	cam->zoom = 1.0;
	cam->vtable = &g_camera_vtable;
	return cam;
}

void camera_destroy(t_camera *camera)
{
	if (camera)
		camera->vtable->destroy(camera);
}