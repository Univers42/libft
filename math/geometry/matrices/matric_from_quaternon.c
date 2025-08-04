/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matric_from_quaternon.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 15:37:44 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/03 20:36:10 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "matrices.h"

/*
** Converts a quaternion (x, y, z, w) to a 4x4 rotation matrix.
** Usage: matrix_from_quaternion(&mat, x, y, z, w);
** Expects: Quaternion components as floats.
*/

void matrix_from_quaternion(t_mat4 *out, float x, float y, float z, float w) {
    float xx;
    float yy;
    float zz;
    float xy;
    float yz;
    float xz;
    float wx;
    float wy;
    float wz;

    xx = x * x;
    yy = y * y;
    zz = z * z;
    xy = x * y;
    xz = x * z;
    yz = y * z;
    wx = w * x;
    wy = w * y;
    wz = w * z;    
    matrix_identity(out);
    out->m[0][0] = 1.0f - 2.0f * (yy + zz);
    out->m[0][1] = 2.0f * (xy - wz);
    out->m[0][2] = 2.0f * (xz + wy);
    out->m[1][0] = 2.0f * (xy + wz);
    out->m[1][1] = 1.0f - 2.0f * (xx + zz);
    out->m[1][2] = 2.0f * (yz - wx);
    out->m[2][0] = 2.0f * (xz - wy);
    out->m[2][1] = 2.0f * (yz + wx);
    out->m[2][2] = 1.0f - 2.0f * (xx + yy);
}
