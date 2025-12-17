/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_create.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 16:18:24 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/09 01:43:32 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_vector.h"
#include "ft_memory.h"

int	vec_create(void *obj, size_t size)
{
	if (!obj || size == 0)
		return (-1);
	ft_memset(obj, 0, size);
	return (0);
}

static size_t	normalize_capacity(const t_vec_config *config)
{
	if (config->initial_capacity == 0)
		return (VECTOR_INITIAL_CAPACITY);
	return (config->initial_capacity);
}

int vec_init(t_vec *v, const t_vec_config *config)
{
	if (!v || !config || config->elem_size == 0)
		return (-1);
	vec_create(v, sizeof(*v));
	v->elem_size = config->elem_size;
	v->type_mask = config->type_mask;
	v->copy_fn = config->copy_fn;
	v->free_fn = config->free_fn;
	v->cap = normalize_capacity(config);
	if (v->cap)
	{
		v->buff = malloc(v->cap * v->elem_size);
		if (!v->buff)
			return (-1);
	}
	return (0);
}
