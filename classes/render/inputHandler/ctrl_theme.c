#include "input_handler.h"
#include "window.h"
#include "point.h"
#include <stdio.h>

// Example: Cycle through terrain colors as themes
static t_terrain g_themes[] = {
    GROUND720, PURPLE1, YELLOW_PALE, GREEN_TERRAIN, PURPLE, AQUA, SHALLOW, DEEP,
    PURPLE2, PURPLE3, PURPLE4, PINK, WHITE1, WHITE2, ORANGE, ORANGE2, ORANGE3,
    YELLOW_TERRAIN, GREEN_PALE, GREEN_PALE2, BLUE_TERRAIN, BLUE2, BLUE3, BLUE4};
static int g_theme_count = sizeof(g_themes) / sizeof(g_themes[0]);
static int g_current_theme = 0;

void handle_theme_key(t_input_handler *handler, t_window *win, int keycode)
{
    (void)win;
    (void)handler;
    if (!win)
        return;
    if (keycode == 't')
    {
        g_current_theme = (g_current_theme + 1) % g_theme_count;
        unsigned int color = g_themes[g_current_theme];
        printf("[THEME] Key 't' pressed: switching to theme color 0x%06x\n", color);
        window_set_background(win, color);
        window_clear(win);
        window_render(win);
    }
    // Add more theme keys if needed
}
