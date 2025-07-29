/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 01:03:47 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/28 01:12:51 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line.h"
#include <stdio.h>

/* Example pixel callback for drawing */
void	draw_pixel(int x, int y, uint32_t color, void *data)
{
	printf("Pixel at (%d, %d) with color 0x%08X\n", x, y, color);
	(void)data; // Suppress unused parameter warning
}

int	main(void)
{
	t_line		*line1;
	t_line		*line2;
	t_line		*clone;
	t_point		*p1;
	t_point		*p2;
	t_point		*interpolated;
	double		length;
	double		slope;

	/* Create points */
	p1 = point_new_with_color(0, 0, 0, 0xFF0000FF); // Red
	p2 = point_new_with_color(100, 50, 10, 0x00FF00FF); // Green
	
	if (!p1 || !p2)
		return (1);

	/* Create line from existing points */
	line1 = line_new(p1, p2);
	if (!line1)
		return (1);

	/* Create line with coordinates and colors */
	line2 = line_new_with_colors(10, 10, 5, 0x0000FFFF,
		90, 90, 15, 0xFFFF00FF);
	if (!line2)
		return (1);

	/* Test line properties */
	length = line1->vtable->get_length(line1);
	slope = line1->vtable->get_slope(line1);
	printf("Line1 length: %.2f, slope: %.2f\n", length, slope);
	printf("Line1 is vertical: %s\n", 
		line1->vtable->is_vertical(line1) ? "true" : "false");
	printf("Line1 is horizontal: %s\n", 
		line1->vtable->is_horizontal(line1) ? "true" : "false");

	/* Color interpolation */
	uint32_t mid_color = line1->vtable->interpolate_color(line1, 0.5);
	printf("Mid-point color: 0x%08X\n", mid_color);

	/* Point interpolation */
	interpolated = line1->vtable->interpolate_point(line1, 0.5);
	if (interpolated)
	{
		t_vec2 coord = interpolated->vtable->get_coordinate(interpolated);
		printf("Interpolated point: (%d, %d, %d)\n", 
			coord.x, coord.y, coord.u_ctx.s_v3.z);
		point_destroy(interpolated);
	}

	/* Draw line using Bresenham algorithm */
	printf("\nDrawing line1 with Bresenham:\n");
	line1->vtable->draw_bresenham(line1, draw_pixel, NULL);

	/* Transform operations */
	printf("\nTransforming line2:\n");
	line2->vtable->translate(line2, 10, 10);
	line2->vtable->scale(line2, 1.5, 1.5);

	/* Clone line */
	clone = line1->vtable->clone(line1);
	if (clone)
	{
		printf("Line cloned successfully\n");
		clone->vtable->destroy(clone);
	}

	/* Distance calculation */
	t_point *test_point = point_new(50, 25, 0);
	if (test_point)
	{
		double distance = line_distance_to_point(line1, test_point);
		printf("Distance from test point to line1: %.2f\n", distance);
		point_destroy(test_point);
	}

	/* Clean up */
	line1->vtable->destroy(line1);
	line2->vtable->destroy(line2);
	point_destroy(p1);
	point_destroy(p2);

	return (0);
}