#include "input_handler.h"
#include "window.h"
#include "camera.h"
#include "point.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

void window_draw_point(t_window *win, int x, int y, unsigned int color);

#define POINT_COUNT 100

typedef struct s_app_ctx
{
    t_window *win;
    t_camera *camera;
    t_point **points;
    int point_count;
} t_app_ctx;

// Helper to create random points (array of pointers)
static void create_random_points(t_point **points, int count, int width, int height)
{
    for (int i = 0; i < count; ++i)
    {
        if (!points[i])
        { // Only create if not already created
            int x = rand() % width;
            int y = rand() % height;
            int z = 0;
            points[i] = point_new(x, y, z);
            if (points[i])
            {
                uint32_t color = 0x010101 * (rand() % 0x1000000);
                points[i]->vtable->set_color(points[i], color);
            }
        }
    }
}

// Redraw callback: clear, project, and draw all points
static int redraw(void *param)
{
    t_app_ctx *ctx = (t_app_ctx *)param;

    // Just clear the window and draw points (no cache/layer/method logic)
    window_clear(ctx->win);

    for (int i = 0; i < ctx->point_count; ++i)
    {
        t_point *pt = ctx->points[i];
        t_vec2 projected = {0};
        if (ctx->camera && ctx->camera->vtable && ctx->camera->vtable->project_point)
            projected = ctx->camera->vtable->project_point(ctx->camera, pt);
        else
            projected = pt->coordinate;

        window_put_pixel(ctx->win, (int)projected.x, (int)projected.y, pt->color.hex_color);
    }
    window_render(ctx->win);
    return 0;
}

int main(void)
{
    srand(time(NULL));

    // Add a background color (e.g. dark blue)
    int bg_color = 0x001122FF;
    t_window *win = window_new(800, 600, "InputHandler Showcase", bg_color);
    if (!win)
    {
        fprintf(stderr, "Failed to create window\n");
        return 1;
    }

    t_camera *camera = camera_new(CAMERA_ISOMETRIC);
    if (!camera)
    {
        fprintf(stderr, "Failed to create camera\n");
        window_destroy(win);
        return 1;
    }

    t_input_handler *handler = input_handler_new(camera);
    if (!handler)
    {
        fprintf(stderr, "Failed to create input handler\n");
        window_destroy(win);
        camera_destroy(camera);
        return 1;
    }

    input_handler_register(win, handler);

    // Create and store points (only once, do not recreate in redraw)
    t_point *points[POINT_COUNT] = {0};
    create_random_points(points, POINT_COUNT, win->width, win->height);

    // Prepare context for redraw
    t_app_ctx *ctx = malloc(sizeof(t_app_ctx));
    ctx->win = win;
    ctx->camera = camera;
    ctx->points = points;
    ctx->point_count = POINT_COUNT;

    // Register redraw callback (called every frame)
    mlx_loop_hook(win->mlx_ptr, redraw, ctx);

    // Show instructions
    printf("Use arrow keys or WASD to move.\n");
    printf("Use +/- to zoom, t to change theme, r/b/c for interface, k/l/m for perspective, mouse to drag/zoom, ESC to exit.\n");
    printf("Random points are displayed with random colors. Move/zoom to interact.\n");

    // Enter the MLX event loop (shows the window and handles events)
    mlx_loop(win->mlx_ptr);

    // Cleanup points and context
    for (int i = 0; i < POINT_COUNT; ++i)
        if (points[i])
            point_destroy(points[i]);
    free(ctx);
    window_destroy(win);
    input_handler_destroy(handler);
    camera_destroy(camera);
    return 0;
}