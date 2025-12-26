/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mat_ops.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 20:30:08 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/26 20:33:12 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "matrices.h"

/**
 * Differences of two vectors
 * 
 */
t_fvec3	fvec3_sub(t_fvec3 a, t_fvec3 b)
{
	return ((t_fvec3)
	{
		.x = a.x - b.x,
		.y = a.y - b.y,
		.z = a.z - b.z
	});
}




/**
 * Computes the cross product of two 3D vectors
 * The cross product gives a vector perpendicular to the two
 * input vectors
 * 
 */
t_fvec3	fvec3_cross(t_fvec3 a, t_fvec3 b)
{
    t_fvec3 cross_product;

    cross_product = (t_fvec3){
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
    return (cross_product);
}

t_fvec3	fvec3_add(t_fvec3 a, t_fvec3 b)
{
	return ((t_fvec3){
		.x = a.x + b.x,
		.y = a.y + b.y,
		.z = a.z + b.z
	});
}

t_fvec3	fvec3_mul(t_fvec3 a, t_fvec3 b)
{
	return ((t_fvec3){
		.x = a.x * b.x,
		.y = a.y * b.y,
		.z = a.z * b.z
	});
}
