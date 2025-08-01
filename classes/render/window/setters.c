/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 17:41:09 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/01 10:36:51 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"

void	window_realloc_screen_buffer(t_window *window, int old_width, int old_height);

void window_update_image(t_window *self)
{
    if (!self || !self->img || !self->win)
        return;
    
    if (self->cache && self->cache->cache_enabled)
    {
        window_cache_smart_update(self);
        // Buffer is already synced in cache logic, just display
        mlx_put_image_to_window(self->mlx, self->win, self->img, 0, 0);
    }
    else
    {
        window_sync_screen_buffer_to_image(self);
        mlx_put_image_to_window(self->mlx, self->win, self->img, 0, 0);
    }
}

void	window_stop_resizing(t_window *self, int width, int height)
{
	if (!self)
		return ;
	self->is_resizing = 0;
	int old_width = self->width;
	int old_height = self->height;
	self->width = width;
	self->height = height;
	if (self->img)
		mlx_destroy_image(self->mlx, self->img);
	self->img = mlx_new_image(self->mlx, width, height);
	window_realloc_screen_buffer(self, old_width, old_height);
	if (self->redraw_cb)
		self->redraw_cb(self->redraw_ctx);
	window_update_image(self);
}

void window_resize(t_window *self, int width, int height)
{
    if (!self)
        return;
    
    int old_width = self->width;
    int old_height = self->height;
    
    // Destroy old cache
    window_cache_destroy(self);
    
    self->width = width;
    self->height = height;
    
    if (self->img)
        mlx_destroy_image(self->mlx, self->img);
    self->img = mlx_new_image(self->mlx, width, height);
    
    window_realloc_screen_buffer(self, old_width, old_height);
    
    // Reinitialize cache with new dimensions
    window_cache_init(self);
    window_cache_render_background(self);
    window_cache_smart_update(self);
}