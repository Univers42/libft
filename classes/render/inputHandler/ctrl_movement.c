#include "input_handler.h"
#include "camera.h"
#include <stdio.h>

// Handle movement key press (arrows, WASD, etc.)
void handle_movement_key_press(t_input_handler *handler, t_window *win, int keycode)
{
    t_key_state *key_state = get_key_state();
    (void)handler;
    (void)win;
    if (keycode == KEY_UP || keycode == 'w' || keycode == 'W')
        key_state->up = 1;
    else if (keycode == KEY_DOWN || keycode == 's' || keycode == 'S')
        key_state->down = 1;
    else if (keycode == KEY_LEFT || keycode == 'a' || keycode == 'A')
        key_state->left = 1;
    else if (keycode == KEY_RIGHT || keycode == 'd' || keycode == 'D')
        key_state->right = 1;
}

// Handle movement key release
void handle_movement_key_release(t_input_handler *handler, t_window *win, int keycode)
{
    t_key_state *key_state = get_key_state();
    (void)handler;
    (void)win;
    if (keycode == KEY_UP || keycode == 'w' || keycode == 'W')
        key_state->up = 0;
    else if (keycode == KEY_DOWN || keycode == 's' || keycode == 'S')
        key_state->down = 0;
    else if (keycode == KEY_LEFT || keycode == 'a' || keycode == 'A')
        key_state->left = 0;
    else if (keycode == KEY_RIGHT || keycode == 'd' || keycode == 'D')
        key_state->right = 0;
}
