/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 23:29:20 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/30 21:03:57 by dlesieur         ###   ########.fr       */
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

typedef struct s_arr_elem
{
	size_t ind;
	char *val;
	struct s_arr_elem *next;
	struct s_arr_elem *prev;
} t_arr_elem;

typedef struct s_array
{
	size_t max_idx;
	size_t nelem;
	size_t first_idx;
	size_t alloc_size;
	struct s_arr_elem **elements;
	struct s_arr_elem *head;
	struct s_arr_elem *lastref;
} t_arr;

#define ARRAY_DFT_SIZE 1024
#define AS_DISPOSE 0x01

void array_resize(t_arr *, size_t);
void array_expand(t_arr *, size_t);
void array_dispose_elements(t_arr_elem **);
t_arr *array_create(void);
void array_flush(t_arr *);
void array_dispose(t_arr *);
t_arr *array_copy(t_arr *a);
t_arr *array_slice(t_arr *, t_arr_elem *, t_arr_elem *);
t_arr *array_slice_safe(t_arr *, size_t, size_t);
void array_walk(t_arr *, void *, void *);
t_arr_elem *array_shift(t_arr *, int, int);
t_arr_elem **array_shift_safe(t_arr *, int, int);
int array_rshift(t_arr *, int, char *);
t_arr_elem *array_unshift_element(t_arr *);
int array_shift_element(t_arr *, char *);
t_arr *array_quote(t_arr *);
t_arr *array_quote_escapes(t_arr *);
t_arr *array_dequote(t_arr *);
t_arr *array_dequote_escapes(t_arr *);
t_arr *array_remove_quoted_nulls(t_arr *);
char *array_subrange(t_arr *, size_t, size_t, int, int, int);
char *array_patsub(t_arr *, char *, char *, int);
char *array_modcase(t_arr *, char *, int, int);
void array_push(t_arr *a, char *v);
/* Basic operations on t_arrayelements. */
t_arr_elem *array_create_element(size_t, char *);
t_arr_elem *array_copy_element(t_arr_elem *);
void array_dispose_element(t_arr_elem *);
int array_insert(t_arr *, size_t, char *);
t_arr_elem *array_remove(t_arr *, size_t);
char *array_reference(t_arr *, size_t);
char **array_to_argv(t_arr *, int *);
t_arr *array_from_argv(t_arr *, char **, int);

char *array_to_kvpair(t_arr *, int);
char *array_to_assign(t_arr *, int);
char *array_to_string(t_arr *, char *, int);
t_arr *array_from_string(char *, char *);
void array_element_replace(t_arr_elem *ae, char *v);
size_t array_num_elements(t_arr *a);
size_t array_max_index(t_arr *a);
size_t array_first_index(t_arr *a);
void array_pop(t_arr *a);
size_t array_alloc_size(t_arr *a);
size_t array_first_index(t_arr *a);
t_arr_elem **array_head(t_arr *a);
bool array_empty(t_arr *a);
char *element_value(t_arr_elem *ae);
size_t element_index(t_arr_elem *ae);
t_arr_elem *element_forw(t_arr_elem *ae);
t_arr_elem *element_back(t_arr_elem *ae);
void set_element_value(t_arr_elem *ae, char *v);
void set_first_index(t_arr *a, size_t i);
void set_num_elements(t_arr *a, size_t n);
void set_max_idx(t_arr *a, size_t i);
t_arr *array_quote(t_arr *array);
void add_before(t_arr_elem *ae, t_arr_elem *new);
void add_after(t_arr_elem *ae, t_arr_elem *new);

/* add lastref helpers prototypes */
t_arr_elem *lastref(t_arr *a);
void set_lastref(t_arr *a, t_arr_elem *e);
void unset_lastref(t_arr *a);
void invalidate_lastref(t_arr *a);
bool is_lastref(t_arr *a);
t_arr_elem *lastref_start(t_arr *a, size_t i);

#endif