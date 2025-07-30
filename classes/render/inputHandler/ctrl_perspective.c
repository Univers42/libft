#include "input_handler.h"
#include "camera.h"
#include <stdio.h>

// Handle perspective change key (calls camera method)
void handle_perspective_key(t_input_handler *handler, t_window *win, int keycode)
{
    (void)win;
    if (!handler || !handler->camera)
        return;
    if (keycode == 'k')
    {
        printf("[PERSPECTIVE] Key 'k' pressed: switching to perspective 1\n");
        handler->camera->vtable->set_perspective(handler->camera, CAMERA_TOP);
    }
    else if (keycode == 'l')
    {
        printf("[PERSPECTIVE] Key 'l' pressed: switching to perspective 2\n");
        handler->camera->vtable->set_perspective(handler->camera, CAMERA_ISOMETRIC);
    }
    else if (keycode == 'm')
    {
        printf("[PERSPECTIVE] Key 'm' pressed: switching to perspective 3\n");
        handler->camera->vtable->set_perspective(handler->camera, CAMERA_SIDE);
    }
    // Add more keys and perspectives as needed
}
