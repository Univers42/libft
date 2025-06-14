/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_clear.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 20:47:32 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/14 22:00:32 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_vector.h"

void    ft_vector_clear(t_vector *vector)
{
    size_t  i;

    if (!vector)
        return;
    
    i = 0;
    while (i < vector->size)
    {
        vector->data[i] = NULL;
        i++;
    }
    vector->size = 0;
}
