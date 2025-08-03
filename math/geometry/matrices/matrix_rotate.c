/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_rotate.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 15:38:16 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/03 15:55:48 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "matrices.h"

/*
** Creates a rotation matrix for rotating by angle_rad radians around axis (x, y, z).
** Usage: matrix_rotate(&mat, angle, x, y, z);
** Expects: Non-zero axis vector; angle in radians.
*/

void matrix_rotate(t_mat4 *out, float angle_rad, float x, float y, float z)
{
	float	c;
	float	s;
	float	t;
	float	mag;

	c = cosf(angle_rad);
	s = sinf(angle_rad);
	t = 1.0f - c;
	mag = sqrtf(x*x + y*y + z*z);
	if (mag == 0.0f)
	{
		matrix_identity(out);
		return ;
	}
	x /= mag; y /= mag; z /= mag;
	matrix_identity(out);
	out->m[0][0] = t*x*x + c;
	out->m[0][1] = t*x*y - s*z;
	out->m[0][2] = t*x*z + s*y;
	out->m[1][0] = t*x*y + s*z;
	out->m[1][1] = t*y*y + c;
	out->m[1][2] = t*y*z - s*x;
	out->m[2][0] = t*x*z - s*y;
	out->m[2][1] = t*y*z + s*x;
	out->m[2][2] = t*z*z + c;
}
