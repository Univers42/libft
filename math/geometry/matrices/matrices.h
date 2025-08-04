/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrices.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 15:37:58 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/04 12:01:24 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATRICES_H
#define MATRICES_H

#include <stddef.h>
#include <math.h>

// 4x4 Matrix and 4D Vector for general 3D transformations
typedef struct {
    float m[4][4];
} t_mat4;

typedef struct {
    float v[4];
} t_vec4;

// Identity and zero
void matrix_identity(t_mat4 *out);
void matrix_zero(t_mat4 *out);

// Copy and transpose
void matrix_copy(t_mat4 *dst, const t_mat4 *src);
void matrix_transpose(t_mat4 *out, const t_mat4 *in);

// Matrix-matrix and matrix-vector multiplication
void matrix_mult(t_mat4 *out, const t_mat4 *a, const t_mat4 *b);
void matrix_vec_multiply(t_vec4 *out, const t_mat4 *mat, const t_vec4 *vec);

// Transformations
void matrix_scale(t_mat4 *out, float sx, float sy, float sz);
void matrix_translate(t_mat4 *out, float tx, float ty, float tz);
void matrix_rotate(t_mat4 *out, float angle_rad, float x, float y, float z);

// Quaternion to matrix
void matrix_from_quaternion(t_mat4 *out, float qx, float qy, float qz, float qw);
void	matrix_init(float (*matrix)[3]);
#endif // MATRICES_H
