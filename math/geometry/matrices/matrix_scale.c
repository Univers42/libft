/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_scale.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 15:38:12 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/26 20:33:39 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "matrices.h"

/*
** Sets the matrix to a scaling transformation with scale factors sx, sy, sz.
** Usage: matrix_scale(&mat, sx, sy, sz);
** Expects: Pointer to a valid t_mat4.
*/
void	matrix_scale(t_mat4 *out, t_fpoint3 s)
{
	matrix_identity(out);
	out->m[0][0] = s.x;
	out->m[1][1] = s.y;
	out->m[2][2] = s.z;
}


/**
 * Scalar multiplication of a vector
 * Multiplies each component of the vector by a scalar value
 */
t_fvec3	fvec3_scale(t_fvec3 v, float scalar)
{
	return ((t_fvec3){
		.x = v.x * scalar,
		.y = v.y * scalar,
		.z = v.z * scalar,
	});
}
