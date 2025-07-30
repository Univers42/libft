#include "input_handler.h"
#include "camera.h"
#include <stdio.h>

// Handle zoom in/out keys (e.g. '+' and '-')
void handle_zoom_key(t_input_handler *handler, t_window *win, int keycode)
{
    if (!handler || !handler->camera)
        return;
    if (keycode == '+' || keycode == 61 || keycode == 65451) // '+' or '=' or numpad '+'
    {
        printf("[ZOOM] Key '+' pressed: zooming in\n");
        handler->camera->vtable->zoom_by(handler->camera, 1.1, win->width / 2, win->height / 2);
    }
    else if (keycode == '-' || keycode == 45 || keycode == 65453) // '-' or numpad '-'
    {
        printf("[ZOOM] Key '-' pressed: zooming out\n");
        handler->camera->vtable->zoom_by(handler->camera, 0.9, win->width / 2, win->height / 2);
    }
    // Add more zoom keys if needed
}
