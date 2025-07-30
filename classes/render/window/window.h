/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:01:09 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/30 04:37:15 by dlesieur         ###   ########.fr       */
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

typedef struct t_window_vtable
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
}	t_window_vtable;

typedef struct s_window_key_state
{
	int	keycode;
	int	is_pressed;
}		t_window_key_state;

typedef struct s_window
{
	const t_window_vtable	*vtable;
	char					*screen_buffer;
	int						width;
	int						height;
	int						startw;
	int						endw;
	int						starth;
	int						endh;
	int						steps;
	int						is_resizing;
	void					*mlx;
	void					*win;
	void					*img;
	t_input_handler			*input_handler;
	int						draw_offset_x;
	int						draw_offset_y;
	void					(*redraw_cb)(void *ctx);
	void					*redraw_ctx;
}	t_window;

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
const t_window_vtable	*get_window_vtable(void);
void					window_realloc_screen_buffer(t_window *self);
#endif