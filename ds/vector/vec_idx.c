/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_idx.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 17:19:07 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/05 18:23:48 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_vector.h"

void *vec_idx(t_vec *v, size_t idx)
{
    if (!v || idx >= v->len)
        return (NULL);
    return ((char *)v->buff + (idx * v->elem_size));
}