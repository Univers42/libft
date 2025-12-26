/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normalize.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 20:16:12 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/26 20:22:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "matrices.h"
#include "algebra.h"

/**
 * @return  the normalized vector (also called a `unit vector`), which
 * has the same direction as teh input but a length of 1.
 * This is called `vector normalization`
 * @note
 * for 3d vector `v = (x, y, z)`, the norm (length) is:
 * `||v|| = sqrt(x² + y² + z²)
 */
t_fvec3	norm(t_fvec3 v)
{
	t_fvec3 result;
	float	length;
	float	ilength;

	result = v;
	length = sqrtf(ft_power(v.x, 2) + ft_power(v.y, 2) + ft_power(v.z, 2));
	if (length != 0.0f)
	{
		ilength = 1.0f / length;
		result.x *= ilength;
		result.y *= ilength;
		result.z *= ilength;
	}
	else
		result.x = 1;
	return (result);
}