/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collection.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:02:14 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/28 11:02:15 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLLECTION_H
#define COLLECTION_H

#include "shapes.h"

// Shape collection structure
typedef struct s_collection {
	t_shape **shapes; // Array of pointers to shapes
	int count; // Number of shapes in the collection
	int capacity; // Maximum capacity of the collection
	char *name; // Collection name
	bool auto_resize; // Whether to automatically resize when full
} t_collection;

// Collection iterator for safe traversal
typedef struct s_collection_iterator {
	t_collection *collection;
	int current_index;
	bool reverse;
} t_collection_iterator;

// Collection statistics
typedef struct s_collection_stats {
	int total_shapes;
	int shape_2d_count;
	int shape_3d_count;
	int visible_count;
	int filled_count;
	size_t total_memory;
} t_collection_stats;

// =============================================================================
// Core collection operations
// =============================================================================

// Constructor/Destructor
t_collection *collection_new(int capacity);
t_collection *collection_new_named(int capacity, const char *name);
void collection_destroy(t_collection *collection);
// Add convenient alias for backward compatibility
void destroy_collection(t_collection *collection);

// Shape management
bool collection_add_shape(t_collection *collection, t_shape *shape);
// Add convenient alias for backward compatibility
bool add_shape(t_collection *collection, t_shape *shape);
bool collection_remove_shape(t_collection *collection, int index);
bool collection_remove_shape_by_ptr(t_collection *collection, t_shape *shape);
void collection_clear(t_collection *collection);

// Access operations
t_shape *collection_get_shape(const t_collection *collection, int index);
int collection_find_shape(const t_collection *collection, const t_shape *shape);
int collection_get_count(const t_collection *collection);
bool collection_is_empty(const t_collection *collection);
bool collection_is_full(const t_collection *collection);

// Batch operations
void collection_draw_all(const t_collection *collection, void *srv, void *win);
void collection_draw_2d_shapes(const t_collection *collection, void *srv, void *win);
void collection_draw_3d_shapes(const t_collection *collection, void *srv, void *win);

// Filtering operations
t_collection *collection_filter_by_type(const t_collection *collection, t_shape_type type);
t_collection *collection_filter_visible(const t_collection *collection);
t_collection *collection_filter_2d(const t_collection *collection);
t_collection *collection_filter_3d(const t_collection *collection);

// Iterator operations
t_collection_iterator *collection_iterator_new(t_collection *collection, bool reverse);
void collection_iterator_destroy(t_collection_iterator *iter);
t_shape *collection_iterator_next(t_collection_iterator *iter);
bool collection_iterator_has_next(const t_collection_iterator *iter);
void collection_iterator_reset(t_collection_iterator *iter);

// Utility operations
bool collection_resize(t_collection *collection, int new_capacity);
t_collection *collection_clone(const t_collection *collection);
void collection_sort_by_type(t_collection *collection);
void collection_reverse(t_collection *collection);

// Statistics and information
t_collection_stats collection_get_stats(const t_collection *collection);
void collection_print_info(const t_collection *collection);
size_t collection_memory_usage(const t_collection *collection);

// Validation
bool collection_is_valid(const t_collection *collection);
bool collection_validate_all_shapes(const t_collection *collection);

#endif
