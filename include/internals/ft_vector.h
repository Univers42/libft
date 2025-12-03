/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vector.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 20:22:34 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/03 23:13:58 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_VECTOR_H
# define FT_VECTOR_H

# include <stdlib.h>
# include <stdbool.h>
# include "ft_stddef.h"
# include "dyn_string.h"

# define VECTOR_INITIAL_CAPACITY 4
# define VECTOR_GROWTH_FACTOR 2

typedef struct s_vector
{
	void	**data;
	size_t	size;
	size_t	capacity;
}			t_vector;

typedef struct s_vec_dyn_str
{
	size_t		cap;
	size_t		len;
	t_dyn_str	*buff;
}	t_vec_dyn_str;

typedef struct s_vec_int
{
	size_t	cap;
	size_t	len;
	int		*buff;
}	t_vec_int;

t_vector	*ft_vector_create(void);
void		ft_vector_destroy(t_vector *vector);
bool		ft_vector_add_back(t_vector *vector, void *element);
void		*ft_vector_pop_back(t_vector *vector);
void		*ft_vector_get(t_vector *vector, size_t index);
bool		ft_vector_set(t_vector *vector, size_t index, void *element);

// Utility functions
size_t		ft_vector_size(t_vector *vector);
size_t		ft_vector_capacity(t_vector *vector);
bool		ft_vector_is_empty(t_vector *vector);
void		ft_vector_clear(t_vector *vector);
bool		ft_vector_resize(t_vector *vector, size_t new_capacity);
bool		ft_vector_reserve(t_vector *vector, size_t capacity);

// Advanced functions
bool		ft_vector_insert(t_vector *vector, size_t index, void *element);
void		*ft_vector_remove(t_vector *vector, size_t index);
void		ft_vector_print_addresses(t_vector *vector);
int			ft_vector_binary_search(t_vector *vector, void *target,
				int (*cmp)(const void *, const void *));
int			vec_int_init(t_vec_int *ret);
int			vec_int_double(t_vec_int *v);
int			vec_int_push(t_vec_int *v, int el);
int			vec_int_pop(t_vec_int *v);
int			vec_int_idx(t_vec_int *v, size_t idx);
int			vec_dyn_str_init(t_vec_dyn_str *ret);
int			vec_dyn_str_double(t_vec_dyn_str *v);
int			vec_dyn_str_push(t_vec_dyn_str *v, t_dyn_str el);
t_dyn_str	vec_dyn_str_pop(t_vec_dyn_str *v);
t_dyn_str	vec_dyn_str_idx(t_vec_dyn_str *v, size_t idx);

#endif
