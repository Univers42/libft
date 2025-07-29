/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 17:19:07 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/29 19:29:06 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_HANDLER_H
# define INPUT_HANDLER_H

# include "window.h"
# include <stdbool.h>

typedef struct s_InputHandler t_InputHandler;

typedef struct s_InputHandlerVTable
{
	void (*on_key_press)(t_InputHandler *self, t_Window *win, int keycode);
	void (*on_key_release)(t_InputHandler *self, t_Window *win, int keycode);
	void (*on_mouse_press)(t_InputHandler *self, t_Window *win, int button, int x, int y);
	void (*on_mouse_release)(t_InputHandler *self, t_Window *win, int button, int x, int y);
	void (*on_mouse_motion)(t_InputHandler *self, t_Window *win, int x, int y);
} t_InputHandlerVTable;

struct s_InputHandler
{
	const t_InputHandlerVTable *vtable;
	// You can add state here if needed
};

extern const t_InputHandlerVTable g_InputHandlerVTable;

t_InputHandler *InputHandler_new(void);
void input_handler_register(t_Window *win, t_InputHandler *handler);
#endif