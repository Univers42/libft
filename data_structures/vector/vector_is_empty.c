/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_is_empty.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 20:51:41 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/14 22:00:30 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_vector.h"

bool    ft_vector_is_empty(t_vector *vector)
{
    if (!vector || vector->size == 0)
        return (true);
    return (false);
}