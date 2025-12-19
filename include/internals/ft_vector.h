/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vector.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 20:22:34 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/19 03:08:45 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_VECTOR_H
#define FT_VECTOR_H

#include <stdlib.h>
#include <stdbool.h>
#include "ft_stddef.h"
#include "dyn_string.h"
#include "ft_memory.h"

#define VECTOR_INITIAL_CAPACITY 4
#define VECTOR_GROWTH_FACTOR 2

/* Type masks (like signal masks) - built-in behaviors */
#define VEC_TYPE_CUSTOM 0x0000 /* user provides copy/free */
#define VEC_TYPE_PTR 0x0001	   /* stores pointers (char *, void *) - free each pointer */
#define VEC_TYPE_DYNSTR 0x0002 /* stores t_dyn_str by value - deep copy/free */
#define VEC_TYPE_PLAIN 0x0004  /* stores plain structs - ft_memcpy, no free */

typedef void (*t_elem_free_fn)(void *elem);
typedef bool (*t_elem_copy_fn)(void *dest, const void *src);

/*
 * Configuration (like sigaction)
 * Either set type_mask for built-in behavior, OR provide custom copy/free
 */
typedef struct s_vec_config
{
	size_t elem_size;
	size_t initial_capacity;
	int type_mask;			/* VEC_TYPE_* flags */
	t_elem_copy_fn copy_fn; /* if type_mask == VEC_TYPE_CUSTOM */
	t_elem_free_fn free_fn; /* if type_mask == VEC_TYPE_CUSTOM */
} t_vec_config;

/*
 * Unified vector - replaces ALL your specialized vectors
 * Always: cap, len, buff (consistent layout)
 */
typedef struct s_vec
{
	size_t			cap;
	size_t			len;
	size_t			count;
	void			*buff; /* typed buffer (char**, t_dyn_str*, t_glob*, etc.) */
	size_t			elem_size;
	int				type_mask;
	t_elem_copy_fn	copy_fn;
	t_elem_free_fn	free_fn;
}	t_vec;

/* Initialization */
/**
 * @brief the functionn vec_create is a api function
 * that accept any struct to be initialized properly..
 * Can be called into a wrapper for personalization.
 * like our own vec_create();
 */
int vec_create(void *obj, size_t size);

/**
 * @brief thin wrapper to vec_create to initialize teh base
 * struct
 */
int vec_init(t_vec *v, const t_vec_config *config);

/**
 *
 */
void vec_destroy(t_vec *v);

/* Core operations */
bool vec_push(t_vec *v, const void *elem);
bool vec_pop(t_vec *v, void *out_elem);
void *vec_get(t_vec *v, size_t index);
bool vec_set(t_vec *v, size_t index, const void *elem);

/* Utilities */
size_t vec_size(const t_vec *v);
size_t vec_capacity(const t_vec *v);
bool vec_is_empty(const t_vec *v);
void vec_clear(t_vec *v);
bool vec_reserve(t_vec *v, size_t capacity);
bool vec_insert(t_vec *v, size_t index, const void *elem);
bool vec_remove(t_vec *v, size_t index, void *out_elem);
void *vec_idx(t_vec *v, size_t idx);

#endif
