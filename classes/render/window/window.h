/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:01:09 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/29 21:09:37 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WINDOW_H
# define WINDOW_H

# define START_W	800
# define START_H	600
# define END_W		400
# define END_H		300
# define STEPS		60

# include "ft_stdlib.h"
# include <stdbool.h>
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
}	t_window;

// Only declare the API, don't define it in the header
t_window				*window_new(int width, int height, const char *title);
void					window_destroy(t_window *self);
void					window_resize(t_window *self, int width, int height);
void					window_init(t_window *self);
void					window_set_resizable(t_window *self);
void					window_close(t_window *self);
const t_window_vtable	*get_window_vtable(void);
#endif