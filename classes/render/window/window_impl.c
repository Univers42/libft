#include "window.h"
#include "object.h" // <-- Add this line to get the full definition of t_object and its vtable
#include <stdio.h>
#include <string.h>

// Make these functions accessible to other files
void window_render_impl(t_window *self)
{
    if (!self)
    {
        printf("❌ window_render_impl: self is NULL\n");
        return;
    }

    if (!self->mlx_ptr)
    {
        printf("❌ window_render_impl: mlx_ptr is NULL\n");
        return;
    }

    if (!self->win_ptr)
    {
        printf("❌ window_render_impl: win_ptr is NULL\n");
        return;
    }

    if (!self->img_ptr)
    {
        printf("❌ window_render_impl: img_ptr is NULL\n");
        return;
    }

    printf("🎨 window_render_impl: Calling mlx_put_image_to_window...\n");
    printf("   mlx_ptr=%p, win_ptr=%p, img_ptr=%p\n", self->mlx_ptr, self->win_ptr, self->img_ptr);

    // Present the frame
    int result = mlx_put_image_to_window(self->mlx_ptr, self->win_ptr, self->img_ptr, 0, 0);
    printf("   mlx_put_image_to_window returned: %d\n", result);

    // Force X11 to process events and display
    printf("🔄 Calling mlx_do_sync for immediate display...\n");
    if (self->mlx_ptr)
    {
        // Force immediate display update
        mlx_do_sync(self->mlx_ptr);
    }

    // Render layers in z-order (from back to front)
    t_layer *layers_sorted[1000];
    int layer_count = 0;

    // Collect all layers
    t_layer *current = self->layers;
    while (current && layer_count < 1000)
    {
        layers_sorted[layer_count++] = current;
        current = current->next;
    }

    // Sort layers by z_index (bubble sort for simplicity)
    for (int i = 0; i < layer_count - 1; i++)
    {
        for (int j = 0; j < layer_count - i - 1; j++)
        {
            if (layers_sorted[j]->z_index > layers_sorted[j + 1]->z_index)
            {
                t_layer *temp = layers_sorted[j];
                layers_sorted[j] = layers_sorted[j + 1];
                layers_sorted[j + 1] = temp;
            }
        }
    }

    // Debug: print layer order
    if (layer_count > 0)
    {
        printf("Rendering %d layers in z-order: ", layer_count);
        for (int i = 0; i < layer_count; i++)
        {
            printf("z=%d ", layers_sorted[i]->z_index);
        }
        printf("\n");
    }

    // Render objects in each layer - use vtable through pointer
    for (int i = 0; i < layer_count; i++)
    {
        t_layer *layer = layers_sorted[i];
        printf("Rendering layer z=%d with %zu objects\n", layer->z_index, layer->object_count);

        for (size_t j = 0; j < layer->object_count; j++)
        {
            t_object *obj = layer->objects[j];
            // Use object through pointer - vtable is accessible
            if (obj && obj->vtable && obj->vtable->draw)
            {
                printf("Drawing object %zu in layer z=%d\n", j, layer->z_index);
                obj->vtable->draw(obj, self);
            }
        }
    }
}

void window_clear_impl(t_window *self)
{
    if (!self || !self->img_data)
    {
        printf("❌ window_clear_impl: Invalid parameters - self=%p, img_data=%p\n",
               self, self ? self->img_data : NULL);
        return;
    }

    printf("🧹 === WINDOW_CLEAR_IMPL DEBUG ===\n");
    printf("   Input bg_color: 0x%06X\n", self->bg_color);
    printf("   Window size: %dx%d\n", self->width, self->height);
    printf("   Image properties: bpp=%d, size_line=%d, endian=%d\n",
           self->bpp, self->size_line, self->endian);

    int total_pixels = self->width * self->height;
    int *pixel_data = (int *)self->img_data;

    printf("   Total pixels to clear: %d\n", total_pixels);
    printf("   Pixel buffer address: %p\n", pixel_data);

    // Fix: Use the color directly for MLX (MLX handles the format internally)
    // For 32-bit MLX images, typically the format is 0x00RRGGBB (RGB)
    unsigned int color = self->bg_color & 0xFFFFFF; // Ensure only RGB bits

    printf("   Color after masking: 0x%06X\n", color);

    // Test different color formats to see what works
    unsigned int test_colors[4];
    test_colors[0] = color;                                                                  // Direct RGB
    test_colors[1] = color | 0xFF000000;                                                     // RGB with alpha
    test_colors[2] = ((color & 0xFF) << 16) | (color & 0xFF00) | ((color & 0xFF0000) >> 16); // BGR
    test_colors[3] = test_colors[2] | 0xFF000000;                                            // BGR with alpha

    printf("   Testing color formats:\n");
    printf("     Format 0 (RGB):       0x%08X\n", test_colors[0]);
    printf("     Format 1 (ARGB):      0x%08X\n", test_colors[1]);
    printf("     Format 2 (BGR):       0x%08X\n", test_colors[2]);
    printf("     Format 3 (ABGR):      0x%08X\n", test_colors[3]);

    // If endian is 0 (little endian), use the color as-is
    // MLX on Linux typically expects ARGB format: 0x00RRGGBB
    unsigned int final_color;
    if (self->endian == 0)
    {
        final_color = color; // Keep RGB as-is
        printf("   Using little-endian format: 0x%08X\n", final_color);
    }
    else
    {
        // Big endian - swap bytes if needed
        unsigned int r = (self->bg_color >> 16) & 0xFF;
        unsigned int g = (self->bg_color >> 8) & 0xFF;
        unsigned int b = self->bg_color & 0xFF;
        final_color = (r) | (g << 8) | (b << 16);
        printf("   Using big-endian format: 0x%08X\n", final_color);
        printf("     r=0x%02X, g=0x%02X, b=0x%02X\n", r, g, b);
    }

    // Clear with final color
    printf("   Writing %d pixels with color 0x%08X...\n", total_pixels, final_color);

    for (int i = 0; i < total_pixels; i++)
    {
        pixel_data[i] = final_color;
    }

    // Verify the write by reading back some pixels
    printf("   Verification - first 3 pixels written:\n");
    for (unsigned int i = 0; i < 3 && i < (unsigned int)total_pixels; i++)
    {
        printf("     pixel[%u] = 0x%08X (expected: 0x%08X) %s\n",
               i, pixel_data[i], final_color,
               (pixel_data[i] == (int)final_color) ? "✅" : "❌");
    }

    // Test writing different colors to small sections for debugging
    if (total_pixels > 1000)
    {
        printf("   Writing test patterns for debugging...\n");

        // Top-left corner: pure red in different formats
        for (int y = 0; y < 50; y++)
        {
            for (int x = 0; x < 50; x++)
            {
                int offset = y * self->width + x;
                pixel_data[offset] = test_colors[0]; // Format 0
            }
        }

        // Top-right corner: pure red in format 1
        for (int y = 0; y < 50; y++)
        {
            for (int x = self->width - 50; x < self->width; x++)
            {
                int offset = y * self->width + x;
                pixel_data[offset] = test_colors[1]; // Format 1
            }
        }

        // Bottom-left corner: pure red in format 2
        for (int y = self->height - 50; y < self->height; y++)
        {
            for (int x = 0; x < 50; x++)
            {
                int offset = y * self->width + x;
                pixel_data[offset] = test_colors[2]; // Format 2
            }
        }

        // Bottom-right corner: pure red in format 3
        for (int y = self->height - 50; y < self->height; y++)
        {
            for (int x = self->width - 50; x < self->width; x++)
            {
                int offset = y * self->width + x;
                pixel_data[offset] = test_colors[3]; // Format 3
            }
        }

        printf("   Test patterns written to corners - check which corner shows red!\n");
    }

    printf("✅ window_clear_impl: Cleared %d pixels (Final color: 0x%08X)\n", total_pixels, final_color);
    printf("=== END WINDOW_CLEAR_IMPL DEBUG ===\n");
}

void window_add_object_impl(t_window *self, t_object *obj, int z_index)
{
    if (!self || !obj)
        return;

    // Find or create layer
    t_layer *layer = self->layers;
    while (layer && layer->z_index != z_index)
    {
        layer = layer->next;
    }

    if (!layer)
    {
        layer = layer_new(z_index);
        if (!layer)
            return;
        layer->next = self->layers;
        self->layers = layer;
    }

    layer_add_object(layer, obj);
}

void window_remove_object_impl(t_window *self, t_object *obj)
{
    if (!self || !obj)
        return;

    t_layer *current = self->layers;
    while (current)
    {
        layer_remove_object(current, obj);
        current = current->next;
    }
}

void window_set_background_impl(t_window *self, int color)
{
    if (!self)
        return;
    self->bg_color = color;
}

void window_resize_impl(t_window *self, int width, int height)
{
    if (!self)
        return;

    self->width = width;
    self->height = height;

    // Recreate image buffer
    if (self->img_ptr)
        mlx_destroy_image(self->mlx_ptr, self->img_ptr);

    self->img_ptr = mlx_new_image(self->mlx_ptr, width, height);
    if (self->img_ptr)
    {
        self->img_data = mlx_get_data_addr(self->img_ptr, &self->bpp,
                                           &self->size_line, &self->endian);
    }
}

int window_handle_events_impl(t_window *self)
{
    if (!self)
        return 0;

    // Basic event handling - for now just return if window should continue
    return !self->should_close;
}
