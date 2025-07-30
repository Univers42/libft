
#include "input_handler.h"
#include "camera.h"
#include <stdio.h>

// Example: Change camera perspective based on key
void handle_perspective_key(t_input_handler *handler, t_window *win, int keycode)
{
    (void)win;
    if (!handler || !handler->camera)
        return;
    if (keycode == 'k')
    {
        // Set to perspective 1 (example)
        printf("[PERSPECTIVE] Key 'k' pressed: switching to perspective 1\n");
        handler->camera->vtable->set_perspective(handler->camera, 1);
    }
    else if (keycode == 'l')
    {
        // Set to perspective 2 (example)
        printf("[PERSPECTIVE] Key 'l' pressed: switching to perspective 2\n");
        handler->camera->vtable->set_perspective(handler->camera, 2);
    }
    else if (keycode == 'm')
    {
        // Set to perspective 3 (example)
        printf("[PERSPECTIVE] Key 'm' pressed: switching to perspective 3\n");
        handler->camera->vtable->set_perspective(handler->camera, 3);
    }
    // Add more keys and perspectives as needed
}
