/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:01:16 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/01 10:36:50 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"

void	window_set_resizable(t_window *self)
{
	XSizeHints	hints;

	if (!self)
		return ;
	hints.flags = PMinSize | PMaxSize | PSize | PPosition;
	hints.min_width = 1;
	hints.min_height = 1;
	hints.max_width = 10000;
	hints.max_height = 10000;
	hints.width = self->width;
	hints.height = self->height;
	hints.x = 0;
	hints.y = 0;
	XSetWMNormalHints(((t_xvar *)self->mlx)->display,
		((t_win_list *)self->win)->window, &hints);
}

void window_set_background_color(t_window *self, unsigned int color)
{
	printf("[DEBUG] window_set_background_color: color=0x%08X\n", color);
	if (!self)
	{
		printf("[DEBUG] window_set_background_color: self is NULL\n");
		return;
	}
	self->background_color = color;
	if (self->cache && self->cache->cache_enabled)
	{
		printf("[DEBUG] window_set_background_color: cache enabled, clearing layer\n");
		window_cache_clear_layer(self, LAYER_BACKGROUND, color);
		window_cache_mark_dirty(self, LAYER_BACKGROUND, 0, 0, self->width, self->height);
	}
}

void window_sync_screen_buffer_to_image(t_window *self)
{
	printf("[DEBUG] window_sync_screen_buffer_to_image called\n");
	if (!self || !self->img || !self->screen_buffer)
		return;
	int bpp, size_line, endian;
	char *img_buf = mlx_get_data_addr(self->img, &bpp, &size_line, &endian);
	if (!img_buf)
		return;
	int total = self->width * self->height;
	unsigned int *src = (unsigned int *)self->screen_buffer;
	unsigned int *dst = (unsigned int *)img_buf;
	for (int i = 0; i < total; ++i)
		dst[i] = src[i];
}

void window_clear(t_window *self)
{
	printf("[DEBUG] window_clear called\n");
	if (!self || !self->screen_buffer)
	{
		printf("[DEBUG] window_clear: self or screen_buffer is NULL\n");
		return;
	}

	if (self->width <= 0 || self->height <= 0 || self->width > 10000 || self->height > 10000)
	{
		fprintf(stderr, "window_clear: invalid window size: width=%d height=%d\n", self->width, self->height);
		return;
	}

	if (self->cache && self->cache->cache_enabled)
	{
		printf("[DEBUG] window_clear: cache enabled, clearing background layer\n");
		window_cache_clear_layer(self, LAYER_BACKGROUND, self->background_color);
		window_cache_mark_dirty(self, LAYER_BACKGROUND, 0, 0, self->width, self->height);
		window_cache_smart_update(self);
		return;
	}

	static unsigned int *bg_row = NULL;
	static int cached_width = 0;
	if (!bg_row || cached_width != self->width)
	{
		if (bg_row)
			free(bg_row);
		bg_row = malloc(sizeof(unsigned int) * self->width);
		cached_width = self->width;
	}
	if (bg_row)
	{
		for (int i = 0; i < self->width; ++i)
			bg_row[i] = self->background_color;
		for (int y = 0; y < self->height; ++y)
			memcpy(&((unsigned int *)self->screen_buffer)[y * self->width], bg_row, sizeof(unsigned int) * self->width);
	}
	else
	{
		int total = self->width * self->height;
		unsigned int *buf = (unsigned int *)self->screen_buffer;
		for (int i = 0; i < total; ++i)
			buf[i] = self->background_color;
	}

	window_sync_screen_buffer_to_image(self);
	if (self->img)
		window_update_image(self);
}

const t_method *get_method(void)
{
    static const t_method vtable = {
        .destroy = window_destroy,
        .resize = window_resize,
        .handle_key = NULL,
        .handle_mouse = NULL,
        .init = window_init,
        .set_resizable = window_set_resizable,
        .close = window_close,
        .get_image_buffer = window_get_image_buffer,
        .update_image = window_update_image,  // Now cache-aware
        .set_background_color = window_set_background_color,
        .clear = window_clear
    };
    return (&vtable);
}

void	*window_get_image_buffer(t_window *self, int *bpp,
			int *size_line, int *endian)
{
	if (!self || !self->img)
		return (NULL);
	return (mlx_get_data_addr(self->img, bpp, size_line, endian));
}

void	window_init(t_window *self)
{
	if (!self)
		return ;
	self->startw = START_W;
	self->starth = START_H;
	self->endh = END_H;
	self->steps = STEPS;
	self->endw = END_W;
}
