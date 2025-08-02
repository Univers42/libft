#include "line.h"
#include "point.h"
#include <math.h>

// Interpolate color between start and end points based on position
uint32_t line_interpolate_color(t_line *self, double t)
{
    if (!self || !self->start || !self->end)
        return 0;

    // Clamp t to [0, 1]
    if (t < 0.0)
        t = 0.0;
    if (t > 1.0)
        t = 1.0;

    t_color start_color, end_color;
    self->start->vtable->get_color(self->start, &start_color);
    self->end->vtable->get_color(self->end, &end_color);

    // Simple linear interpolation between colors
    uint8_t r1 = (start_color.hex_color >> 24) & 0xFF;
    uint8_t g1 = (start_color.hex_color >> 16) & 0xFF;
    uint8_t b1 = (start_color.hex_color >> 8) & 0xFF;
    uint8_t a1 = start_color.hex_color & 0xFF;

    uint8_t r2 = (end_color.hex_color >> 24) & 0xFF;
    uint8_t g2 = (end_color.hex_color >> 16) & 0xFF;
    uint8_t b2 = (end_color.hex_color >> 8) & 0xFF;
    uint8_t a2 = end_color.hex_color & 0xFF;

    uint8_t r = (uint8_t)(r1 + t * (r2 - r1));
    uint8_t g = (uint8_t)(g1 + t * (g2 - g1));
    uint8_t b = (uint8_t)(b1 + t * (b2 - b1));
    uint8_t a = (uint8_t)(a1 + t * (a2 - a1));

    return (r << 24) | (g << 16) | (b << 8) | a;
}

// Translate line by dx, dy (int)
bool line_translate(t_line *self, int dx, int dy)
{
    if (!self || !self->start || !self->end)
        return false;

    // Translate both start and end points
    self->start->vtable->translate(self->start, dx, dy);
    self->end->vtable->translate(self->end, dx, dy);
    return true;
}

// Scale line around center point by scale_x, scale_y (double)
bool line_scale(t_line *self, double scale_x, double scale_y)
{
    if (!self || !self->start || !self->end)
        return false;

    // Calculate center point
    double center_x = (self->start->pos.x + self->end->pos.x) / 2.0;
    double center_y = (self->start->pos.y + self->end->pos.y) / 2.0;

    // Translate to origin, scale, then translate back
    line_translate(self, -center_x, -center_y);

    self->start->pos.x = self->start->pos.x * scale_x;
    self->start->pos.y = self->start->pos.y * scale_y;
    self->end->pos.x = self->end->pos.x * scale_x;
    self->end->pos.y = self->end->pos.y * scale_y;

    line_translate(self, center_x, center_y);
    return true;
}

// Get angle of line in radians
double line_get_angle(t_line *self)
{
    if (!self || !self->start || !self->end)
        return 0.0;

    double dx = self->end->pos.x - self->start->pos.x;
    double dy = self->end->pos.y - self->start->pos.y;

    return atan2(dy, dx);
}
