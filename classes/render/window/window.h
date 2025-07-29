/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:01:09 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/29 19:44:23 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WINDOW_H
#define WINDOW_H

# define START_W	800
# define START_H	600
# define END_W		400
# define END_H		300
# define STEPS		60

# include "ft_stdlib.h"
# include <stdbool.h>
// Forward declaration for t_server if needed
typedef struct s_server t_server;
typedef struct s_InputHandler t_InputHandler; // <-- Add this line
typedef struct s_Window t_Window;

typedef struct s_WindowVTable
{
	void (*destroy)			(t_Window *self);
	void (*resize)			(t_Window *self, int width, int height);
	void (*handle_key)		(t_Window *self, int keycode);
	void (*handle_mouse)	(t_Window *self, int button, int x, int y, int action);
	void (*init)			(t_Window *self);
	void (*set_resizable)	(t_Window *self);
	void (*close)			(t_Window *self); // <-- Add this
}							t_WindowVTable;

typedef struct s_WindowKeyState
{
	int	keycode;
	int	is_pressed;
}		t_WindowKeyState;

struct s_Window
{
	const t_WindowVTable	*vtable;
	// Add t_server *server if you use it in your implementation
	// t_server				*server;
	char					*screen_buffer;
	int						width;
	int						height;
	int						startw;
	int						endw;
	int						starth;
	int						endh;
	int						steps;
	int						is_resizing; // Used as "key is held" flag
	void					*mlx;
	void					*win;
	void					*img;
	t_InputHandler			*input_handler; // <-- fix type here
	// t_WindowKeyState		key_state; // Uncomment if used
};

// Only declare the vtable, define it in a .c file
extern const t_WindowVTable g_WindowVTable;

// Only declare the API, don't define it in the header
t_Window	*Window_new(int width, int height, const char *title);
void		Window_destroy(t_Window *self);
void		Window_resize(t_Window *self, int width, int height);
void		Window_init(t_Window *self);
void		Window_set_resizable(t_Window *self);
void		Window_close(t_Window *self);

#endif