/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 23:29:20 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/30 16:27:18 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARRAY_H
#define ARRAY_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct s_arr_elem t_arr_elem;

typedef struct s_array
{
	size_t max_idx;
	size_t nelem;
	size_t first_idx;
	size_t alloc_size;
	struct s_arr_elem **elements;
	struct s_arr_elem *head;
	struct s_arr_elem *lastref;
} t_array;

typedef struct s_arr_elem
{
	size_t ind;
	char *val;
	struct s_arr_elem *next;
	struct s_arr_elem *prev;
} t_arr_elem;

# define ARRAY_DFT_SIZE 1024
# define AS_DISPOSE 0x01

void array_resize(t_array *, size_t);
void array_expand(t_array *, size_t);
void array_dispose_elements(t_arr_elem **);
t_array *array_create(void);
void array_flush(t_array *);
void array_dispose(t_array *);
t_array *array_copy(t_array *a);
t_array *array_slice(t_array *, t_arr_elem *, t_arr_elem *);
t_array *array_slice_safe(t_array *, size_t, size_t);
void array_walk(t_array *, void *, void *);
t_arr_elem *array_shift(t_array *, int, int);
t_arr_elem **array_shift_safe(t_array *, int, int);
int array_rshift(t_array *, int, char *);
t_arr_elem *array_unshift_element(t_array *);
int array_shift_element(t_array *, char *);
t_array *array_quote(t_array *);
t_array *array_quote_escapes(t_array *);
t_array *array_dequote(t_array *);
t_array *array_dequote_escapes(t_array *);
t_array *array_remove_quoted_nulls(t_array *);
char *array_subrange(t_array *, size_t, size_t, int, int, int);
char *array_patsub(t_array *, char *, char *, int);
char *array_modcase(t_array *, char *, int, int);
void array_push(t_array *a, char *v);
/* Basic operations on t_arrayelements. */
t_arr_elem *array_create_element(size_t, char *);
t_arr_elem *array_copy_element(t_arr_elem *);
void array_dispose_element(t_arr_elem *);
int array_insert(t_array *, size_t, char *);
t_arr_elem *array_remove(t_array *, size_t);
char *array_reference(t_array *, size_t);
char **array_to_argv(t_array *, int *);
t_array *array_from_argv(t_array *, char **, int);

char *array_to_kvpair(t_array *, int);
char *array_to_assign(t_array *, int);
char *array_to_string(t_array *, char *, int);
t_array *array_from_string(char *, char *);
void array_element_replace(t_arr_elem *ae, char *v);
size_t array_num_elements(t_array *a);
size_t array_max_index(t_array *a);
size_t array_first_index(t_array *a);
void array_pop(t_array *a);
size_t array_alloc_size(t_array *a);
size_t array_first_index(t_array *a);
t_arr_elem **array_head(t_array *a);
bool array_empty(t_array *a);
char *element_value(t_arr_elem *ae);
size_t element_index(t_arr_elem *ae);
t_arr_elem *element_forw(t_arr_elem *ae);
t_arr_elem *element_back(t_arr_elem *ae);
void set_element_value(t_arr_elem *ae, char *v);
void set_first_index(t_array *a, size_t i);
void set_num_elements(t_array *a, size_t n);
void set_max_idx(t_array *a, size_t i);
t_array *array_quote(t_array *array);
/* Flags for array_shift */

void add_before(t_arr_elem *ae, t_arr_elem *new);
void add_after(t_arr_elem *ae, t_arr_elem *new);

/* add lastref helpers prototypes */
t_arr_elem	*lastref(t_array *a);
void		set_lastref(t_array *a, t_arr_elem *e);
void		unset_lastref(t_array *a);
void		invalidate_lastref(t_array *a);
bool		is_lastref(t_array *a);
t_arr_elem	*lastref_start(t_array *a, size_t i);

#endif