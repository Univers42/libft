/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quaternion.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 15:37:35 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/04 14:28:33 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUATERNION_H
#define QUATERNION_H

#include <math.h>
#include "pixel.h"
#include "matrices.h"

/*
 * Given two rotations, e1 and e2, expressed as quaternion rotations,
 * figure out the equivalent single rotation and stuff it into dest.
 *
 * This routine also normalizes the result every RENORMCOUNT times it is
 * called, to keep error from creeping in.
 *
 * NOTE: This routine is written so that q1 or q2 may be the same
 * as dest (or each other).
 */

#define TRACKBALLSIZE  (0.5f)
#define RENORMCOUNT 97
// Quaternion struct definition
typedef struct s_quat
{
	float x;
	float y;
	float z;
	float w;
} t_quat;

typedef struct s_slerp_interp
{
	float	theta_0;
	float	theta;
	float	sin_theta;
	float	sin_theta0;
	float	s0;
	float	s1;
}			t_slerp_interp;

float	lerp(float a, float b, float t);
// Initialization
void	quat_init(t_quat *q, float x, float y, float z, float w);
void	quat_from_axis_angle(t_quat *q, float axis_x, float axis_y, float axis_z, float angle_rad);

// Operations
void	quat_normalize(t_quat *q);
void	quat_mult(t_quat *out, const t_quat *a, const t_quat *b);
void	quat_slerp(t_quat *out, const t_quat *a, const t_quat *b, float t);

// Conversion
void	quat_to_matrix(t_mat4 *out, const t_quat *q);
void	quaternion_to_matrix(float q[4], float m[3][3]);
void	apply_trackball_matrix(float m[3][3], t_point *point);

//vector methods
void	vcopy(const float *v1, float *v2);
void	vscale(float *v, float div);
void	vnormal(float *v);
void	vset(float *v, float x, float y, float z);
void	vsub(const float *src1, const float *src2, float *dst);
void	vzero(float *v);
float	vlength(const float *v);
float	vdot(const float *v1, const float *v2);
void	vcross(const float *v1, const float *v2, float *cross);
void	vadd(const float *src1, const float *src2, float *dst);

void	add_quats(float q1[4], float q2[4], float dest[4]);
void	axis_to_quat(float a[3], float phi, float q[4]);
void	normalize_quat(float q[4]);
void	apply_trackball_matrix(float m[3][3], t_point *point);
void quat_from_axis_angle(t_quat *q, float axis_x, float axis_y, float axis_z, float angle_rad);
void	quat_init(t_quat *q, float x, float y, float z, float w);
void quat_mult(t_quat *out, const t_quat *a, const t_quat *b);
void	normalize_quat(float q[4]);
void	quat_slerp(t_quat *out, const t_quat *a, const t_quat *b, float t);
void	quaternion_to_matrix(float q[4], float m[3][3]);
void	build_rotmatrix(float m[4][4], float q[4]);
void	trackball_rotate(float q[4], float p1x, float p1y, float p2x, float p2y);
void	build_rotmatrix(float m[4][4], float q[4]);
float    tb_project_to_sphere(float r, float x, float y);

#endif
