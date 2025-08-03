/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quaternion.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 15:37:35 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/03 15:37:36 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUATERNION_H
#define QUATERNION_H

#include <math.h>
#include "../../matrices/matrices.h"

typedef struct s_vec4
{
	float	x,
	float	y,
	float	z,
	float	w;
}			t_vec4;

inline float lerp(float a, float b, float t)
{
    return a + t * (b - a);
}

// Initialization
void quat_init(t_quat *q, float x, float y, float z, float w);
void quat_from_axis_angle(t_quat *q, float axis_x, float axis_y, float axis_z, float angle_rad);

// Operations
void quat_normalize(t_quat *q);
void quat_mult(t_quat *out, const t_quat *a, const t_quat *b);
void quat_slerp(t_quat *out, const t_quat *a, const t_quat *b, float t);

// Conversion
void quat_to_matrix(t_mat4 *out, const t_quat *q);

#endif // QUATERNION_H
