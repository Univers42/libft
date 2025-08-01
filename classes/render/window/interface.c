/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:01:23 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/01 09:04:45 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"
#include <stdlib.h>
#include "input_handler.h"
#include "point.h"
#include "camera.h"

#define DRAW_OFFSET_X 300
#define DRAW_OFFSET_Y 200

void	window_init(t_window *self);
void	window_set_resizable(t_window *self);

void	window_close(t_window *self)
{
	if (!self)
		return ;
	self->method->destroy(self);
	exit(0);
}

/**
 * @brief for who's asking why we're using window as handler of
 * event like zooming when we have a camera. It's because 
 * the camera should be responsible for the zoom logic itself
 * (the math, state, effect on the view), but the window
 * (or event system) should be responsible for handling the mouse
 * wheel event and deciding when to call the camera's zoom function.
 * The camera knows how to zoom but should not know about events, mouse
 * or windowing
 * The camera only provide a clean API for zooming, pannign,etc..
 * @param self structure of window itself clalback
 * @param point x and y member
 * @param delta difference between point
 * @param ctrl type of ctrl
 * @return void just do the action no error expected
 */
void window_handle_mouse_wheel(t_window *self, t_vectorN point, int delta, int ctrl)
{
    t_camera *camera;
    double factor;

    if (!self || !self->input_handler)
        return;
    
    camera = input_handler_get_camera(self->input_handler);
    if (!camera)
        return;
    
    if (ctrl)
    {
        if (delta > 0)
            factor = 1.1;
        else
            factor = 0.9;
        
        camera->vtable->zoom_by(camera, factor,
            point.x - self->draw_offset_x, point.y - self->draw_offset_y);
        
        // NEW: Update cache viewport
        if (self->cache)
        {
            double zoom = camera->zoom;
            int cam_x = (int)camera->offset_x;
            int cam_y = (int)camera->offset_y;
            window_cache_update_viewport(self, cam_x, cam_y, zoom);
        }
    }
}

static void	window_redraw_callback(void *ctx)
{
	t_window *win = (t_window *)ctx;

	if (!win)
		return ;
	win->method->clear(win);
	win->method->update_image(win);
}

static int	window_init_members(t_window *win, int width,
				int height, const char *title)
{
	win->method = get_method();
	win->width = width;
	win->height = height;
	win->is_resizing = 0;
	win->mlx = mlx_init();
	if (!win->mlx)
		return (0);
	if (title)
		win->win = mlx_new_window(win->mlx, width, height, (char *)(title));
	else
		win->win = mlx_new_window(win->mlx, width, height, "Window");
	if (!win->win)
		return (0);
	win->img = mlx_new_image(win->mlx, width, height);
	if (!win->img)
		return (0);
	win->screen_buffer = malloc(sizeof(unsigned int) * width * height);
	if (win->screen_buffer)
		ft_memset(win->screen_buffer, 0, sizeof(unsigned int) * width * height);
	win->draw_offset_x = 300;
	win->draw_offset_y = 200;
	win->redraw_cb = window_redraw_callback;
	win->redraw_ctx = win;
	return (1);
}

t_window *window_new(int width, int height, const char *title)
{
    t_window *win;

    win = malloc(sizeof(t_window));
    if (!win)
        return (NULL);
    
    if (!window_init_members(win, width, height, title))
    {
        if (win->img)
            mlx_destroy_image(win->mlx, win->img);
        if (win->win)
            mlx_destroy_window(win->mlx, win->win);
        if (win->mlx)
            free(win->mlx);
        free(win);
        return (NULL);
    }
    
    // NEW: Initialize cache system
    if (!window_cache_init(win))
    {
        window_destroy(win);
        return (NULL);
    }
    
    return (win);
}

void window_destroy(t_window *self)
{
    if (!self)
        return;
    
    // NEW: Destroy cache first
    window_cache_destroy(self);
    
    if (self->mlx && self->img)
        mlx_destroy_image(self->mlx, self->img);
    if (self->mlx && self->win)
        mlx_destroy_window(self->mlx, self->win);
    if (self->screen_buffer)
        free(self->screen_buffer);
    free(self);
}
