/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 17:19:07 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/29 21:58:49 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_HANDLER_H
# define INPUT_HANDLER_H

# include "window.h"
# include <stdbool.h>

typedef struct s_camera t_camera; // Add forward declaration
typedef struct s_input_handler t_input_handler;

typedef struct s_input_handlerVTable
{
	void (*on_key_press)(t_input_handler *self, t_window *win, int keycode);
	void (*on_key_release)(t_input_handler *self, t_window *win, int keycode);
	void (*on_mouse_press)(t_input_handler *self, t_window *win, int button, int x, int y);
	void (*on_mouse_release)(t_input_handler *self, t_window *win, int button, int x, int y);
	void (*on_mouse_motion)(t_input_handler *self, t_window *win, int x, int y);
} t_input_handlerVTable;

struct s_input_handler
{
	const t_input_handlerVTable *vtable;
	t_camera *camera; // Add camera pointer
	// You can add state here if needed
};

extern const t_input_handlerVTable g_InputHandlerVTable;

t_input_handler *InputHandler_new(t_camera *camera); // Update constructor signature
void input_handler_register(t_window *win, t_input_handler *handler);
#endif