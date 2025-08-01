/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cache_system.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 08:47:09 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/01 10:40:50 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"

#include <sys/time.h>

// Get current timestamp in milliseconds
static unsigned long get_timestamp(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

// Move this to file scope, before its usage
static void cache_pixel_callback(int x, int y, uint32_t color, void *data)
{
    t_window *w = ((void**)data)[0];
    t_layer_type ly = *((t_layer_type*)((void**)data)[1]);
    window_cache_draw_point(w, ly, x, y, color);
}

// Initialize cache system
int window_cache_init(t_window *win)
{
    if (!win)
        return 0;
    
    win->cache = malloc(sizeof(t_window_cache));
    if (!win->cache)
        return 0;
    
    t_window_cache *cache = win->cache;
    memset(cache, 0, sizeof(t_window_cache));
    
    // Initialize each layer
    for (int i = 0; i < LAYER_COUNT; i++)
    {
        cache->layers[i].img = mlx_new_image(win->mlx, win->width, win->height);
        if (!cache->layers[i].img)
        {
            window_cache_destroy(win);
            return 0;
        }
        
        int bpp, size_line, endian;
        cache->layers[i].buffer = mlx_get_data_addr(cache->layers[i].img, 
                                                   &bpp, &size_line, &endian);
        
        cache->layers[i].is_visible = 1;
        cache->layers[i].needs_full_redraw = 1;
        cache->layers[i].dirty_rect.is_dirty = 0;
    }
    
    // Create composite image
    cache->composite_img = mlx_new_image(win->mlx, win->width, win->height);
    if (!cache->composite_img)
    {
        window_cache_destroy(win);
        return 0;
    }
    
    int bpp, size_line, endian;
    cache->composite_buffer = mlx_get_data_addr(cache->composite_img, 
                                               &bpp, &size_line, &endian);
    
    cache->cache_enabled = 1;
    cache->zoom_level = 1.0;
    
    return 1;
}

// Destroy cache system
void window_cache_destroy(t_window *win)
{
    if (!win || !win->cache)
        return;
    
    t_window_cache *cache = win->cache;
    
    for (int i = 0; i < LAYER_COUNT; i++)
    {
        if (cache->layers[i].img)
            mlx_destroy_image(win->mlx, cache->layers[i].img);
    }
    
    if (cache->composite_img)
        mlx_destroy_image(win->mlx, cache->composite_img);
    
    free(cache);
    win->cache = NULL;
}

// Mark a region as dirty for a specific layer
void window_cache_mark_dirty(t_window *win, t_layer_type layer, 
                            int x, int y, int width, int height)
{
    if (!win || !win->cache || layer >= LAYER_COUNT)
        return;
    
    t_cache_layer *l = &win->cache->layers[layer];
    
    if (!l->dirty_rect.is_dirty)
    {
        // First dirty region
        l->dirty_rect.x = x;
        l->dirty_rect.y = y;
        l->dirty_rect.width = width;
        l->dirty_rect.height = height;
        l->dirty_rect.is_dirty = 1;
    }
    else
    {
        // Expand dirty region to include new area
        int x1 = l->dirty_rect.x;
        int y1 = l->dirty_rect.y;
        int x2 = x1 + l->dirty_rect.width;
        int y2 = y1 + l->dirty_rect.height;
        
        int new_x1 = (x < x1) ? x : x1;
        int new_y1 = (y < y1) ? y : y1;
        int new_x2 = (x + width > x2) ? x + width : x2;
        int new_y2 = (y + height > y2) ? y + height : y2;
        
        l->dirty_rect.x = new_x1;
        l->dirty_rect.y = new_y1;
        l->dirty_rect.width = new_x2 - new_x1;
        l->dirty_rect.height = new_y2 - new_y1;
    }
    
    l->last_update = get_timestamp();
}

// Clear a layer to a specific color
void window_cache_clear_layer(t_window *win, t_layer_type layer, unsigned int color)
{
    if (!win || !win->cache || layer >= LAYER_COUNT)
        return;
    
    t_cache_layer *l = &win->cache->layers[layer];
    unsigned int *buf = (unsigned int *)l->buffer;
    int total = win->width * win->height;
    
    for (int i = 0; i < total; i++)
        buf[i] = color;
    
    l->needs_full_redraw = 0;
    l->dirty_rect.is_dirty = 0;
    l->last_update = get_timestamp();
}

// Optimized point drawing with cache awareness
void window_cache_draw_point(t_window *win, t_layer_type layer, 
                            int x, int y, unsigned int color)
{
    if (!win || !win->cache || layer >= LAYER_COUNT)
        return;
    
    if (x < 0 || y < 0 || x >= win->width || y >= win->height)
        return;
    
    t_cache_layer *l = &win->cache->layers[layer];
    unsigned int *buf = (unsigned int *)l->buffer;
    
    buf[y * win->width + x] = color;
    
    // Mark single pixel as dirty
    window_cache_mark_dirty(win, layer, x, y, 1, 1);
}

// Optimized line drawing with cache awareness
void window_cache_draw_line_to_layer(t_window *win, t_layer_type layer, t_line *line)
{
    if (!win || !win->cache || !line || layer >= LAYER_COUNT)
        return;
    
    // Get line bounds for dirty region calculation
    t_point *p1 = line->start;
    t_point *p2 = line->end;
    
    int min_x = (p1->pos.x < p2->pos.x) ? p1->pos.x : p2->pos.x;
    int min_y = (p1->pos.y < p2->pos.y) ? p1->pos.y : p2->pos.y;
    int max_x = (p1->pos.x > p2->pos.x) ? p1->pos.x : p2->pos.x;
    int max_y = (p1->pos.y > p2->pos.y) ? p1->pos.y : p2->pos.y;
    
    // Draw line using existing method but to cache layer
    void *callback_data[2] = {win, &layer};
    line->vtable->draw_bresenham(line, cache_pixel_callback, callback_data);
    
    // Mark line area as dirty
    window_cache_mark_dirty(win, layer, min_x, min_y, 
                           max_x - min_x + 1, max_y - min_y + 1);
}

// Composite all visible layers
void window_cache_composite(t_window *win)
{
    if (!win || !win->cache)
        return;
    
    t_window_cache *cache = win->cache;
    unsigned int *composite = (unsigned int *)cache->composite_buffer;
    int total = win->width * win->height;

    // Start by copying the background layer to composite buffer
    unsigned int *bg_buf = (unsigned int *)cache->layers[LAYER_BACKGROUND].buffer;
    memcpy(composite, bg_buf, total * sizeof(unsigned int));

    // Blend other layers in order (skip background)
    for (int layer = 1; layer < LAYER_COUNT; layer++)
    {
        if (!cache->layers[layer].is_visible)
            continue;
        
        unsigned int *layer_buf = (unsigned int *)cache->layers[layer].buffer;
        for (int i = 0; i < total; i++)
        {
            unsigned int pixel = layer_buf[i];
            // Only overwrite if pixel is not fully transparent
            if ((pixel & 0xFF000000) != 0)
            {
                composite[i] = pixel;
            }
        }
    }
}

// Smart update - only recomposite if layers changed
void window_cache_smart_update(t_window *win)
{
    printf("[DEBUG] window_cache_smart_update called\n");
    if (!win || !win->cache || !win->cache->cache_enabled)
    {
        // Fallback to original update
        window_update_image(win);
        return;
    }
    
    t_window_cache *cache = win->cache;
    int needs_composite = 0;
    
    // Check if any layer needs updating
    for (int i = 0; i < LAYER_COUNT; i++)
    {
        if (cache->layers[i].dirty_rect.is_dirty || cache->layers[i].needs_full_redraw)
        {
            needs_composite = 1;
            cache->layers[i].dirty_rect.is_dirty = 0;
            cache->layers[i].needs_full_redraw = 0;
            cache->cache_misses++;
        }
    }
    
    if (needs_composite)
    {
        window_cache_composite(win);
        memcpy(win->screen_buffer, win->cache->composite_buffer, 
               win->width * win->height * sizeof(unsigned int));
        window_sync_screen_buffer_to_image(win); // <-- Ensure bulk update
    }
    else
    {
        cache->cache_hits++;
    }
    
    mlx_put_image_to_window(win->mlx, win->win, win->img, 0, 0);
}

// Viewport change detection and cache invalidation
void window_cache_update_viewport(t_window *win, int x, int y, double zoom)
{
    if (!win || !win->cache)
        return;
    
    t_window_cache *cache = win->cache;
    
    // Check if viewport significantly changed
    int dx = abs(cache->viewport_x - x);
    int dy = abs(cache->viewport_y - y);
    double zoom_diff = fabs(cache->zoom_level - zoom);
    
    if (dx > 10 || dy > 10 || zoom_diff > 0.01)
    {
        // Invalidate map layer cache
        cache->layers[LAYER_MAP].needs_full_redraw = 1;
        cache->map_cache_valid = 0;
        
        cache->viewport_x = x;
        cache->viewport_y = y;
        cache->zoom_level = zoom;
    }
}

// Background caching for static elements
void window_cache_render_background(t_window *win)
{
    printf("[DEBUG] window_cache_render_background called\n");
    if (!win || !win->cache)
    {
        printf("[DEBUG] window_cache_render_background: win or cache is NULL\n");
        return;
    }
    t_window_cache *cache = win->cache;
    window_cache_clear_layer(win, LAYER_BACKGROUND, win->background_color);
    window_cache_mark_dirty(win, LAYER_BACKGROUND, 0, 0, win->width, win->height);
    cache->background_cached = 1;
    printf("[DEBUG] window_cache_render_background: marked dirty and cached\n");
}

// Usage example integration
void enhanced_redraw_callback(void *ctx)
{
    t_window *win = (t_window *)ctx;

    if (!win || !win->cache)
    {
        // Fallback to original method
        win->method->clear(win);
        win->method->update_image(win);
        return;
    }

    // Only render background if it is not cached
    // Remove repeated calls!
    // if (!win->cache->background_cached)
    //     window_cache_render_background(win);

    // Clear dynamic layers only
    window_cache_clear_layer(win, LAYER_OBJECTS, 0x00000000);  // Transparent
    window_cache_clear_layer(win, LAYER_UI, 0x00000000);      // Transparent

    // Redraw dynamic content to appropriate layers
    // Example: draw current objects to LAYER_OBJECTS

    // Smart composite and update (only once per frame)
    window_cache_smart_update(win);
}