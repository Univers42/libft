#include "input_handler.h"
#include <stdio.h>

// Forward declarations for handler functions
void handle_movement_key_press(t_input_handler *handler, t_window *win, int keycode);
void handle_movement_key_release(t_input_handler *handler, t_window *win, int keycode);
void handle_perspective_key(t_input_handler *handler, t_window *win, int keycode);
void handle_zoom_key(t_input_handler *handler, t_window *win, int keycode);
void handle_interface_key(t_input_handler *handler, t_window *win, int keycode);
void handle_theme_key(t_input_handler *handler, t_window *win, int keycode);

// Entry point for event dispatching
void dispatch_event(t_input_handler *handler, t_window *win, int keycode, int is_press)
{
    int mask = get_event_mask(keycode);
    if (mask & EVENT_MOVE)
    {
        if (is_press)
            handle_movement_key_press(handler, win, keycode);
        else
            handle_movement_key_release(handler, win, keycode);
    }
    if (mask & EVENT_CAMERA)
    {
        handle_perspective_key(handler, win, keycode);
    }
    if (mask & EVENT_ZOOM)
    {
        handle_zoom_key(handler, win, keycode);
    }
    if (mask & EVENT_WINDOW)
    {
        handle_interface_key(handler, win, keycode);
    }
    if (mask & EVENT_EXIT)
    {
        // Example: close window on exit event
        if (win)
            window_destroy(win);
    }
    if (mask & EVENT_MOUSE)
    {
        // Mouse events handled elsewhere
    }
    // Theme event (e.g. 't')
    if (keycode == 't' || keycode == 'T')
        handle_theme_key(handler, win, keycode);
}
