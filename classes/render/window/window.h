/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:01:09 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/01 10:50:30 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WINDOW_H
# define WINDOW_H

# include "point.h"
# include "mlx.h"
# include <stdbool.h>
# include "window.h"
# include <string.h>
# include "ft_stdlib.h"
# include "mlx_int.h"
# include <X11/Xlib.h>
# include <X11/Xutil.h>
# include "camera.h"
# include "input_handler.h"
# include "line.h"
# include <stdint.h> // <-- Add this line

// Forward declaration for t_line to resolve circular dependency
struct s_line;
typedef struct s_line t_line;

// Fallback Defaults
# define DEFAULT_START_W 800
# define DEFAULT_START_H 600
# define DEFAULT_END_W   400
# define DEFAULT_END_H   300
# define DEFAULT_STEPS   60

# ifndef START_W
#  define START_W DEFAULT_START_W
# endif
# ifndef START_H
#  define START_H DEFAULT_START_H
# endif
# ifndef END_W
#  define END_W DEFAULT_END_W
# endif
# ifndef END_H
#  define END_H DEFAULT_END_H
# endif
# ifndef STEPS
#  define STEPS DEFAULT_STEPS
# endif

# if (START_W < 100) || (START_H < 100)|| (END_W < 100) \
|| (END_H < 100) || (STEPS < 60)
#  undef START_W
#  undef START_H
#  undef END_W
#  undef END_H
#  undef STEPS
#  define START_W DEFAULT_START_W
#  define START_H DEFAULT_START_H
#  define END_W   DEFAULT_END_W
#  define END_H   DEFAULT_END_H
#  define STEPS   DEFAULT_STEPS
# endif

// Forward declaration for t_server if needed
typedef struct s_server			t_server;
typedef struct s_input_handler	t_input_handler;
typedef struct s_window			t_window;

typedef enum e_layer_type
{
    LAYER_BACKGROUND = 0,  // Static background (rarely changes)
    LAYER_MAP,            // Map data (changes on pan/zoom)
    LAYER_OBJECTS,        // Dynamic objects (frequent updates)
    LAYER_UI,             // UI elements (overlays, menus)
    LAYER_COUNT
} t_layer_type;

// Dirty region tracking
typedef struct s_dirty_rect
{
    int x, y, width, height;
    int is_dirty;
} t_dirty_rect;

// Cache layer structure
typedef struct s_cache_layer
{
    void *img;                    // MLX image for this layer
    char *buffer;                 // Raw pixel buffer
    t_dirty_rect dirty_rect;      // Dirty region tracking
    int is_visible;               // Layer visibility
    int needs_full_redraw;        // Force complete redraw flag
    unsigned long last_update;    // Timestamp for cache invalidation
} t_cache_layer;

// Enhanced window structure with caching
typedef struct s_window_cache
{
    t_cache_layer layers[LAYER_COUNT];
    void *composite_img;          // Final composited image
    char *composite_buffer;       // Composite buffer
    int viewport_x, viewport_y;   // Current viewport position
    double zoom_level;            // Current zoom level
    int cache_enabled;            // Global cache toggle
    
    // Optimization flags
    int background_cached;        // Background is cached and valid
    int map_cache_valid;          // Map cache is valid for current view
    
    // Performance counters (optional)
    int cache_hits;
    int cache_misses;
} t_window_cache;

typedef struct s_method
{
	void	(*destroy)(t_window *self);
	void	(*resize)(t_window *self, int width, int height);
	void	(*handle_key)(t_window *self, int keycode);
	void	(*handle_mouse)(t_window *self, int button,
			int x, int y, int action);
	void	(*init)(t_window *self);
	void	(*set_resizable)(t_window *self);
	void	(*close)(t_window *self);
	void	*(*get_image_buffer)(t_window *self, int *bpp,
			int *size_line, int *endian);
	void	(*update_image)(t_window *self);
	void	(*set_background_color)(t_window *self, unsigned int color);
	void	(*clear)(t_window *self);
}	t_method;

typedef struct s_window_key_state
{
	int	keycode;
	int	is_pressed;
}		t_window_key_state;

typedef struct s_window
{
    const t_method          *method;
    char                    *screen_buffer;
    t_window_cache          *cache;           // NEW: Cache system
    int                     width;
    int                     height;
    int                     startw;
    int                     endw;
    int                     starth;
    int                     endh;
    int                     steps;
    int                     is_resizing;
    void                    *mlx;
    void                    *win;
    void                    *img;
    void                    *img_bg;         // Keep existing layers for compatibility
    void                    *img_fg;
    void                    *img_ui;
    t_input_handler         *input_handler;
    int                     draw_offset_x;
    int                     draw_offset_y;
    void                    (*redraw_cb)(void *ctx);
    void                    *redraw_ctx;
    unsigned int            background_color;
} t_window;


// Only declare the API, don't define it in the header
t_window				*window_new(int width, int height, const char *title);
void					window_destroy(t_window *self);
void					window_resize(t_window *self, int width, int height);
void					window_init(t_window *self);
void					window_set_resizable(t_window *self);
void					window_close(t_window *self);
void					*window_get_image_buffer(t_window *self,
							int *bpp, int *size_line, int *endian);
void					window_update_image(t_window *self);
void					window_start_resizing(t_window *self);
void					window_stop_resizing(t_window *self,
							int width, int height);
void					window_poll_resize(t_window *self);
const t_method			*get_method(void);
void					window_realloc_screen_buffer(t_window *self, int old_width, int old_height);
// Draw a line into the window's image buffer
void					window_draw_line(t_window *win, t_line *line);
void					window_set_background_color(t_window *self, unsigned int color);
void					window_sync_screen_buffer_to_image(t_window *self);

// Cache API prototypes
int window_cache_init(t_window *win);
void window_cache_destroy(t_window *win);
void window_cache_smart_update(t_window *win);
void window_cache_render_background(t_window *win);
void window_cache_draw_line_to_layer(t_window *win, t_layer_type layer, t_line *line);
void window_cache_draw_point(t_window *win, t_layer_type layer, int x, int y, unsigned int color);
void window_cache_update_viewport(t_window *win, int cam_x, int cam_y, double zoom);
void window_cache_clear_layer(t_window *win, t_layer_type layer, unsigned int color);
void window_cache_mark_dirty(t_window *win, t_layer_type layer, int x, int y, int width, int height);
void window_cache_smart_update(t_window *win);

#endif