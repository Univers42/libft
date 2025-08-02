/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 17:19:07 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/30 15:52:39 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_HANDLER_H
# define INPUT_HANDLER_H

#include "camera.h"
#include "mlx.h"
#include "mlx_int.h"
#include "ft_math.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// Define event categories as bitwise flags
typedef enum e_event_category
{
	EVENT_NONE    = 0,
	EVENT_MOVE    = 1 << 0,
	EVENT_CAMERA  = 1 << 1,
	EVENT_ZOOM    = 1 << 2,
	EVENT_WINDOW  = 1 << 3,
	EVENT_MOUSE   = 1 << 4,
	EVENT_EXIT    = 1 << 5,
	EVENT_ALL     = 0x3F
}	t_event_category;

// Mouse button codes (X11)
#ifndef MOUSE_LEFT
# define MOUSE_LEFT   1
#endif
#ifndef MOUSE_MIDDLE
# define MOUSE_MIDDLE 2
#endif
#ifndef MOUSE_RIGHT
# define MOUSE_RIGHT  3
#endif
#ifndef MOUSE_SCROLL_UP
# define MOUSE_SCROLL_UP 4
#endif
#ifndef MOUSE_SCROLL_DOWN
# define MOUSE_SCROLL_DOWN 5
#endif

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
	void (*handle_camera_top)(t_input_handler *self, t_window *win);
    void (*handle_camera_isometric)(t_input_handler *self, t_window *win);
    void (*handle_camera_side)(t_input_handler *self, t_window *win);
    void (*handle_move_left_press)(t_input_handler *self, t_window *win);
    void (*handle_move_right_press)(t_input_handler *self, t_window *win);
    void (*handle_move_up_press)(t_input_handler *self, t_window *win);
    void (*handle_move_down_press)(t_input_handler *self, t_window *win);
    void (*handle_move_left_release)(t_input_handler *self, t_window *win);
    void (*handle_move_right_release)(t_input_handler *self, t_window *win);
    void (*handle_move_up_release)(t_input_handler *self, t_window *win);
    void (*handle_move_down_release)(t_input_handler *self, t_window *win);
    void (*handle_zoom_in)(t_input_handler *self, t_window *win);
    void (*handle_zoom_out)(t_input_handler *self, t_window *win);
    void (*handle_start_resize)(t_input_handler *self, t_window *win);
    void (*handle_stop_resize)(t_input_handler *self, t_window *win);
    void (*handle_close_window)(t_input_handler *self, t_window *win);
} t_input_handlerVTable;

typedef struct s_key_state
{
	int up;
	int down;
	int left;
	int right;
} t_key_state;

struct s_input_handler
{
	const t_input_handlerVTable *vtable;
	t_camera *camera; // Add camera pointer
	// You can add state here if needed
};

extern const t_input_handlerVTable g_InputHandlerVTable;

t_input_handler *input_handler_new(t_camera *camera);
void			input_handler_destroy(t_input_handler *handler);
void			input_handler_register(t_window *win, t_input_handler *handler);

// Add keycode definitions for arrow keys (X11/Linux)
# ifndef KEY_UP
#  define KEY_UP		65362
# endif
# ifndef KEY_DOWN
#  define KEY_DOWN	65364
# endif
# ifndef KEY_LEFT
#  define KEY_LEFT	65361
# endif
# ifndef KEY_RIGHT
#  define KEY_RIGHT	65363
# endif

typedef int (*t_verif)(int keycode);

// Add this declaration to make input_handler_get_camera visible everywhere:
t_camera		*input_handler_get_camera(t_input_handler *handler);
int				mlx_mouse_release_cb(int button, int x, int y, void *param);
int				mlx_mouse_motion_cb(int x, int y, void *param);
int				mlx_mouse_press_cb(int button, int x, int y, void *param);
int				mlx_key_press_cb(int keycode, void *param);
int				mlx_key_release_cb(int keycode, void *param);
int				key_repeat_loop(void *param);
void 			input_handler_register(t_window *win, t_input_handler *handler);
void 			input_handler_destroy(t_input_handler *handler);
int 			mlx_destroy_notify_cb(void *param);
t_input_handler	*input_handler_new(t_camera *camera);
void			on_key_press(t_input_handler *self, t_window *win, int keycode);
void			on_key_release(t_input_handler *self, t_window *win, int keycode);
void			on_mouse_motion(t_input_handler *self, t_window *win, int x, int y);
void			on_mouse_release(int button);
void			on_mouse_press(t_input_handler *self, t_window *win, int button, int x, int y);
void			update_movement(t_camera *camera);
t_key_state		*get_key_state(void);

// Key category detection functions
int				what_category(int keycode, t_event_category category);
int				is_e_zoom(int keycode);
int				is_e_win(int keycode);
int				is_e_mouse(int keycode);
int				is_e_exit(int keycode);
int				is_e_interface(int keycode);
t_verif			is_e_move(int keycode);
t_verif			is_e_camera(int keycode);
int get_event_mask(int keycode);
#endif