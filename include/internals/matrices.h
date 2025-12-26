/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrices.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 15:37:58 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/26 23:44:50 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATRICES_H
# define MATRICES_H

# include <stddef.h>
# include <math.h>
# include "pixel.h"

# define D3 3
# define D2 2

enum e_coordinate
{
	COORD_X,
	COORD_Y,
	COORD_Z,
	COORD_W,
	COORD_TOTAL
};

typedef struct s_mat4
{
	float	m[COORD_TOTAL][COORD_TOTAL];
}			t_mat4;

typedef struct s_vec4
{
	float	v[COORD_TOTAL];
}			t_vec4;

typedef struct s_fvec2
{
	float	x;
	float	y;
	float	coord[COORD_TOTAL - 2];
}	t_fvec2;

typedef struct s_fvec3
{
	float	x;
	float	y;
	float	z;
	float	coord[COORD_TOTAL - 1]
}	t_fvec3;

// Identity and zero
void	matrix_identity(t_mat4 *out);
void	matrix_zero(t_mat4 *out);

// Copy and transpose
void	matrix_copy(t_mat4 *dst, const t_mat4 *src);
void	matrix_transpose(t_mat4 *out, const t_mat4 *in);

// Matrix-matrix and matrix-vector multiplication
void	matrix_mult(t_mat4 *out, const t_mat4 *a, const t_mat4 *b);
void	matrix_vec_multiply(t_vec4 *out, const t_mat4 *mat, const t_vec4 *vec);

// Transformations
void	matrix_scale(t_mat4 *out, t_fpoint3 s);
void	matrix_translate(t_mat4 *out, t_fpoint3 t);
void	matrix_rotate(t_mat4 *out, float angle_rad, t_fpoint3 p);

// Quaternion to matrix
void	matrix_from_quaternion(t_mat4 *out, t_fpoint4 q);
void	matrix_init(float (*matrix)[3]);

t_fvec3	fvec3_scale(t_fvec3 v, float scalar);
t_fvec3	norm(t_fvec3 v);
float	fvec3_len_sq(t_fvec3 v);
t_fvec3	fvec3_invert(t_fvec3 v);
t_fvec3	fvec3_scale(t_fvec3 v, float scalar);
t_fvec3	vec3_rotate_yaw_pitch(t_fvec3 v, float yaw, float pitch);
t_fvec2	vec2_rotate(t_fvec2 v, float angle);
float	fvec3_dot(t_fvec3 a, t_fvec3 b);
float	fvec3_idx(t_fvec3 v, int idx);
t_fvec3	fvec3_sub(t_fvec3 a, t_fvec3 b);
t_fvec3	fvec3_cross(t_fvec3 a, t_fvec3 b);
t_fvec3	fvec3_add(t_fvec3 a, t_fvec3 b);
t_fvec3	fvec3_mul(t_fvec3 a, t_fvec3 b);
void sub3(const float a[3], const float b[3], float out[3]);
float dot3(const float a[3], const float b[3]);
void cross3(const float a[3], const float b[3], float out[3]);
void sub3(const float a[3], const float b[3], float out[3]);
float len3(const float v[3]);
void normalize3(const float v[3], float out[3]);
void normalize3(const float v[3], float out[3]);

#endif // MATRICES_H
