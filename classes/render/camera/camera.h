/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:03:41 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/30 02:27:28 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_H
#define CAMERA_H

#include "window.h"
#include "point.h"
#include "input_handler.h"
#include <unistd.h>
#include "mlx.h"
#include <X11/Xlib.h>      // Add this line
#include "mlx_int.h"       // Add this line for t_xvar and t_win_list
#include <stdlib.h>
#include <math.h>

#define NUM_POINTS 6
/**
 * Treating the camera as an object is important for modularity,
 * encapsulation and flexibility when rendering the system. 
 * The camera can be easily test independently from rendering and input.
 * We can extend the logic and calculation easily (projection, zoom..)
 * view
 * 
 * The camera object is the abstraction for "How the user sees teh world"
 * it is not responsible for drawing, input, or event handling,
 * but for all math and state related to the view and projection.
 */

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

typedef struct s_app
{
	t_window	*win;
	t_camera	*camera;
	t_point		*points[NUM_POINTS];
} t_app;

t_camera *camera_new(t_camera_perspective perspective);
void camera_destroy(t_camera *camera);

#endif
