/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_ops.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 20:22:34 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/26 20:26:27 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "matrices.h"
#include "algebra.h"

/**
 * Computes the squares length (norm squared) of vector
 * Mathematical cncept: `Squared Euclidean norm`
 * Used for performaces (avoid sqrt), when we only need to compare lengths
 * usage: used n normalization
 */
float	fvec3_len_sq(t_fvec3 v)
{
	return (ft_power(v.x, 2), ft_power(v.y, 2), ft_power(v.z, 2));
}

/**
 * @return the inverse (negation) of a vector
 * Mathematical concept: Additive inverse(opposite direction)
 */
t_fvec3	fvec3_invert(t_fvec3 v)
{
	return ((t_fvec3)
	{
		.x = -v.x,
		.y = -v.y,
		.z = -v.z
	});
}