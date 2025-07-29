/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stat.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:01:56 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/28 11:01:57 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "collection.h"

// =============================================================================
// Statistics and information
// =============================================================================

t_collection_stats collection_get_stats(const t_collection *collection) {
	t_collection_stats stats = {0};
	
	if (!collection)
		return stats;
	
	stats.total_shapes = collection->count;
	
	for (int i = 0; i < collection->count; i++) {
		if (collection->shapes[i]) {
			if (shape_is_2d(collection->shapes[i])) {
				stats.shape_2d_count++;
			} else if (shape_is_3d(collection->shapes[i])) {
				stats.shape_3d_count++;
			}
			
			if (collection->shapes[i]->properties.visible) {
				stats.visible_count++;
			}
			
			if (collection->shapes[i]->properties.filled) {
				stats.filled_count++;
			}
			
			stats.total_memory += sizeof(t_shape);
		}
	}
	
	stats.total_memory += sizeof(t_collection);
	stats.total_memory += sizeof(t_shape *) * collection->capacity;
	
	return stats;
}

void collection_print_info(const t_collection *collection) {
	if (!collection) {
		printf("Collection: NULL\n");
		return;
	}
	
	printf("Collection Information:\n");
	printf("  Name: %s\n", collection->name ? collection->name : "Unnamed");
	printf("  Count: %d/%d\n", collection->count, collection->capacity);
	printf("  Auto-resize: %s\n", collection->auto_resize ? "Yes" : "No");
	
	t_collection_stats stats = collection_get_stats(collection);
	printf("  2D Shapes: %d\n", stats.shape_2d_count);
	printf("  3D Shapes: %d\n", stats.shape_3d_count);
	printf("  Visible: %d\n", stats.visible_count);
	printf("  Filled: %d\n", stats.filled_count);
	printf("  Memory Usage: %zu bytes\n", stats.total_memory);
	
	// Print individual shapes
	printf("  Shapes:\n");
	for (int i = 0; i < collection->count; i++) {
		if (collection->shapes[i]) {
			const char *type_name = shape_get_type_name(collection->shapes[i]);
			t_position pos = shape_get_position(collection->shapes[i]);
			int color = shape_get_color(collection->shapes[i]);
			printf("    [%d] %s at (%.1f, %.1f, %.1f) color=0x%06X %s\n", 
				   i, type_name, pos.x, pos.y, pos.z, color,
				   collection->shapes[i]->properties.visible ? "visible" : "hidden");
		}
	}
}

size_t collection_memory_usage(const t_collection *collection) {
	if (!collection)
		return 0;
	
	t_collection_stats stats = collection_get_stats(collection);
	return stats.total_memory;
}
