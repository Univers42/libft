/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:03:41 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/30 00:34:36 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_H
#define CAMERA_H

#include "../point/point.h"
#include "../window/window.h"

typedef enum e_camera_perspective
{
	CAMERA_ISOMETRIC,
	CAMERA_TOP,
	CAMERA_SIDE
}	t_camera_perspective;

typedef struct s_camera t_camera;

typedef struct s_camera_vtable
{
	void (*set_perspective)(t_camera *self, t_camera_perspective perspective);
	t_camera_perspective (*get_perspective)(t_camera *self);
	t_vec2 (*project_point)(t_camera *self, t_point *point);
	void (*move)(t_camera *self, double dx, double dy);    // Pan
	void (*set_zoom)(t_camera *self, double zoom, double center_x, double center_y);         // Set zoom centralized
	void (*zoom_by)(t_camera *self, double factor, double center_x, double center_y);        // Zoom in/out centralized
	void (*destroy)(t_camera *self);
}	t_camera_vtable;

struct s_camera
{
	t_camera_perspective	perspective;
	double					offset_x;   // Pan horizontally
	double					offset_y;   // Pan vertically
	double					zoom;       // Zoom factor
	t_camera_vtable			*vtable;
};

t_camera *camera_new(t_camera_perspective perspective);
void camera_destroy(t_camera *camera);

#endif

