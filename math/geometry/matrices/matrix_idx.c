/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_idx.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 20:07:16 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/26 20:32:49 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "matrices.h"

float	fvec3_idx(t_fvec3 v, int idx)
{
	if (idx == 0)
		return (v.x);
	if (idx == 1)
		return (v.y);
	if (idx == 2)
		return (v.z);
	return (0);
}

/**
 * it returns a single float value.
 * the result measures how much the two vectors point in the same dir
 * if the result is positive, the vectors point in a similar direction
 * if `zero`, the vectors are perpendicular (orhtogonal)
 * 
 * usage:
 * - calculating angles between vectors
 * - Projecting one vector onto another
 * - Lighting caclulations in graphics
 */
float	fvec3_dot(t_fvec3 a, t_fvec3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}