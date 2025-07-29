/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:02:18 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/28 11:02:19 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "collection.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// =============================================================================
// Constructor/Destructor
// =============================================================================

t_collection *collection_new(int capacity) {
	return collection_new_named(capacity, "Unnamed Collection");
}

t_collection *collection_new_named(int capacity, const char *name) {
	if (capacity <= 0)
		return NULL;
	
	t_collection *col = malloc(sizeof(t_collection));
	if (!col)
		return NULL;
	
	col->shapes = malloc(sizeof(t_shape *) * capacity);
	if (!col->shapes) {
		free(col);
		return NULL;
	}
	
	// Initialize all pointers to NULL
	for (int i = 0; i < capacity; i++) {
		col->shapes[i] = NULL;
	}
	
	col->count = 0;
	col->capacity = capacity;
	col->auto_resize = true;
	
	// Set name
	if (name) {
		col->name = malloc(strlen(name) + 1);
		if (col->name) {
			strcpy(col->name, name);
		}
	} else {
		col->name = NULL;
	}
	
	return col;
}

void collection_destroy(t_collection *collection) {
	if (!collection)
		return;
	
	// Destroy all shapes
	collection_clear(collection);
	
	// Free the shapes array
	if (collection->shapes) {
		free(collection->shapes);
	}
	
	// Free the name
	if (collection->name) {
		free(collection->name);
	}
	
	// Free the collection itself
	free(collection);
}

// Add convenient alias for backward compatibility
void destroy_collection(t_collection *collection) {
	collection_destroy(collection);
}

// =============================================================================
// Shape management
// =============================================================================

bool collection_add_shape(t_collection *collection, t_shape *shape) {
	if (!collection || !shape || !shape_is_valid(shape))
		return false;
	
	// Check if we need to resize
	if (collection->count >= collection->capacity) {
		if (collection->auto_resize) {
			if (!collection_resize(collection, collection->capacity * 2)) {
				return false;
			}
		} else {
			return false; // Collection is full
		}
	}
	
	collection->shapes[collection->count] = shape;
	collection->count++;
	return true;
}

// Add convenient alias for backward compatibility
bool add_shape(t_collection *collection, t_shape *shape) {
	return collection_add_shape(collection, shape);
}

bool collection_remove_shape(t_collection *collection, int index) {
	if (!collection || index < 0 || index >= collection->count)
		return false;
	
	// Destroy the shape
	if (collection->shapes[index]) {
		shape_destroy(collection->shapes[index]);
	}
	
	// Shift remaining shapes down
	for (int i = index; i < collection->count - 1; i++) {
		collection->shapes[i] = collection->shapes[i + 1];
	}
	
	collection->count--;
	collection->shapes[collection->count] = NULL;
	return true;
}

bool collection_remove_shape_by_ptr(t_collection *collection, t_shape *shape) {
	if (!collection || !shape)
		return false;
	
	int index = collection_find_shape(collection, shape);
	if (index >= 0) {
		return collection_remove_shape(collection, index);
	}
	return false;
}

void collection_clear(t_collection *collection) {
	if (!collection)
		return;
	
	for (int i = 0; i < collection->count; i++) {
		if (collection->shapes[i]) {
			shape_destroy(collection->shapes[i]);
			collection->shapes[i] = NULL;
		}
	}
	collection->count = 0;
}


// =============================================================================
// Batch operations
// =============================================================================

void collection_draw_all(const t_collection *collection, void *srv, void *win) {
	if (!collection || !srv)
		return;
	
	for (int i = 0; i < collection->count; i++) {
		if (collection->shapes[i] && shape_is_valid(collection->shapes[i])) {
			shape_draw(collection->shapes[i], srv, win);
		}
	}
}

void collection_draw_2d_shapes(const t_collection *collection, void *srv, void *win) {
	if (!collection || !srv)
		return;
	
	for (int i = 0; i < collection->count; i++) {
		if (collection->shapes[i] && shape_is_valid(collection->shapes[i])) {
			if (shape_is_2d(collection->shapes[i])) {
				shape_draw(collection->shapes[i], srv, win);
			}
		}
	}
}

void collection_draw_3d_shapes(const t_collection *collection, void *srv, void *win) {
	if (!collection || !srv)
		return;
	
	for (int i = 0; i < collection->count; i++) {
		if (collection->shapes[i] && shape_is_valid(collection->shapes[i])) {
			if (shape_is_3d(collection->shapes[i])) {
				shape_draw(collection->shapes[i], srv, win);
			}
		}
	}
}

// =============================================================================
// Iterator operations
// =============================================================================

t_collection_iterator *collection_iterator_new(t_collection *collection, bool reverse) {
	if (!collection)
		return NULL;
	
	t_collection_iterator *iter = malloc(sizeof(t_collection_iterator));
	if (!iter)
		return NULL;
	
	iter->collection = collection;
	iter->reverse = reverse;
	iter->current_index = reverse ? collection->count - 1 : 0;
	
	return iter;
}


t_shape *collection_iterator_next(t_collection_iterator *iter) {
	if (!iter || !collection_iterator_has_next(iter))
		return NULL;
	
	t_shape *shape = iter->collection->shapes[iter->current_index];
	
	if (iter->reverse) {
		iter->current_index--;
	} else {
		iter->current_index++;
	}
	
	return shape;
}

bool collection_iterator_has_next(const t_collection_iterator *iter) {
	if (!iter || !iter->collection)
		return false;
	
	if (iter->reverse) {
		return iter->current_index >= 0;
	} else {
		return iter->current_index < iter->collection->count;
	}
}

void collection_iterator_reset(t_collection_iterator *iter) {
	if (!iter || !iter->collection)
		return;
	
	iter->current_index = iter->reverse ? iter->collection->count - 1 : 0;
}

// =============================================================================
// Utility operations
// =============================================================================

bool collection_resize(t_collection *collection, int new_capacity) {
	if (!collection || new_capacity <= 0 || new_capacity < collection->count)
		return false;
	
	t_shape **new_shapes = realloc(collection->shapes, sizeof(t_shape *) * new_capacity);
	if (!new_shapes)
		return false;
	
	collection->shapes = new_shapes;
	
	// Initialize new slots to NULL
	for (int i = collection->capacity; i < new_capacity; i++) {
		collection->shapes[i] = NULL;
	}
	
	collection->capacity = new_capacity;
	return true;
}

t_collection *collection_clone(const t_collection *collection) {
	if (!collection)
		return NULL;
	
	t_collection *cloned = collection_new_named(collection->capacity, collection->name);
	if (!cloned)
		return NULL;
	
	cloned->auto_resize = collection->auto_resize;
	
	// Clone all shapes
	for (int i = 0; i < collection->count; i++) {
		if (collection->shapes[i]) {
			t_shape *shape_clone_ptr = shape_clone(collection->shapes[i]);
			if (shape_clone_ptr) {
				collection_add_shape(cloned, shape_clone_ptr);
			}
		}
	}
	
	return cloned;
}

// Simple comparison function for sorting by type
static int compare_shapes_by_type(const void *a, const void *b) {
	t_shape *shape_a = *(t_shape **)a;
	t_shape *shape_b = *(t_shape **)b;
	
	if (!shape_a || !shape_b)
		return 0;
	
	return (int)shape_get_type(shape_a) - (int)shape_get_type(shape_b);
}

void collection_sort_by_type(t_collection *collection) {
	if (!collection || collection->count <= 1)
		return;
	
	qsort(collection->shapes, collection->count, sizeof(t_shape *), compare_shapes_by_type);
}

void collection_reverse(t_collection *collection) {
	if (!collection || collection->count <= 1)
		return;
	
	for (int i = 0; i < collection->count / 2; i++) {
		t_shape *temp = collection->shapes[i];
		collection->shapes[i] = collection->shapes[collection->count - 1 - i];
		collection->shapes[collection->count - 1 - i] = temp;
	}
}


// =============================================================================
// Validation
// =============================================================================

bool collection_is_valid(const t_collection *collection) {
	if (!collection || !collection->shapes)
		return false;
	
	if (collection->count < 0 || collection->capacity <= 0)
		return false;
	
	if (collection->count > collection->capacity)
		return false;
	
	return true;
}

bool collection_validate_all_shapes(const t_collection *collection) {
	if (!collection_is_valid(collection))
		return false;
	
	for (int i = 0; i < collection->count; i++) {
		if (!collection->shapes[i] || !shape_is_valid(collection->shapes[i])) {
			return false;
		}
	}
	
	return true;
}

// =============================================================================
// Convenience functions for easier usage
// =============================================================================

void collection_draw_all_simple(const t_collection *collection, void *srv) {
	collection_draw_all(collection, srv, NULL);
}

// Print a summary of collection contents
void collection_summary(const t_collection *collection) {
	if (!collection) {
		printf("Collection: NULL\n");
		return;
	}
	
	printf("Collection '%s': %d/%d shapes\n", 
		   collection->name ? collection->name : "Unnamed",
		   collection->count, collection->capacity);
}
