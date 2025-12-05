/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_ensure_space.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 17:27:20 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/05 18:23:45 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_vector.h"

/* Deprecated: use vec_reserve instead. Kept for backwards compatibility. */
int vec_ensure_space(t_vec *v)
{
    size_t new_cap;

    if (!v)
        return (-1);
    new_cap = (v->cap ? v->cap * VECTOR_GROWTH_FACTOR : VECTOR_INITIAL_CAPACITY);
    if (!vec_reserve(v, new_cap))
        return (-1);
    return (0);
}
