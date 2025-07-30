/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 17:19:07 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/30 04:32:42 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_HANDLER_H
# define INPUT_HANDLER_H

# include "window.h"
# include <stdbool.h>

/**
 * @brief `inputHandler` acts like  a bridge between low-level input events
 * and high level actions into the application (like moving the camera,..)
 * Key responsabilities of this class
 * - Listen for input events (key presses, mouse movements, etc..)
 * from the graphic server.
 * - interpret those events and translate them into meaningful action
 * for applications
 * call the appropriate methods on other objects (like the camera, window)
 * to perform those actions..
 * Maintain input state (e.g., which keys are currently pressed for continuous
 * movement or diagonal movement).
 * 
 * Why this organization ?
 * it keeps the code modular: the window, camera and other classes don't need
 * to know about raw input events.
 * it allows us to change input mapping or add new controls without modifying
 * the core logic
 * it makes the code easier to maintain extend and test.
 */

 
typedef struct s_camera t_camera; // Add forward declaration
typedef struct s_window t_window; // <-- Add this forward declaration
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

t_input_handler *InputHandler_new(t_camera *camera);
void InputHandler_destroy(t_input_handler *handler);
void input_handler_register(t_window *win, t_input_handler *handler);

// Add keycode definitions for arrow keys (X11/Linux)
#ifndef KEY_UP
# define KEY_UP     65362
#endif
#ifndef KEY_DOWN
# define KEY_DOWN   65364
#endif
#ifndef KEY_LEFT
# define KEY_LEFT   65361
#endif
#ifndef KEY_RIGHT
# define KEY_RIGHT  65363
#endif

// Add this declaration to make input_handler_get_camera visible everywhere:
t_camera *input_handler_get_camera(t_input_handler *handler);

#endif