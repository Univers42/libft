/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linear_interpolation.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 20:43:54 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/26 20:47:04 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "matrices.h"
#include "algebra.h"

t_fvec3	fvec3_lerp(t_fvec3 v1, t_fvec3 v2, float prop)
{
    return (t_fvec3){
        .x = ft_lerp(v1.x, v2.x, prop),
        .y = ft_lerp(v1.y, v2.y, prop),
        .z = ft_lerp(v1.z, v2.z, prop),
    };
}
