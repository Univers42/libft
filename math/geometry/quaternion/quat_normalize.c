/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quat_normalize.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 14:08:50 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/05 02:33:56 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quaternion.h"
#include <math.h>

/* normalize a 4-float array in-place */
static void normalize_quat_arr(float q[4])
{
	int i;
	float mag;

	mag = sqrtf(q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
	if (mag == 0.0f)
		return;
	i = 0;
	while (i < 4)
	{
		q[i] /= mag;
		i++;
	}
}

/* Provide the C-style symbol expected elsewhere (float[4]) */
void normalize_quat(float q[4])
{
	normalize_quat_arr(q);
}

/* Public API expected by headers */
void quat_normalize(t_quat *q)
{
	/* treat the t_quat storage as 4 contiguous floats */
	normalize_quat_arr((float *)q);
}
