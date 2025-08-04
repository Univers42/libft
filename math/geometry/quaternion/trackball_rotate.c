/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trackball_rotate.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 14:08:18 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/04 14:08:32 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quaternion.h"

void	trackball_rotate(float q[4], float p1x, float p1y, float p2x, float p2y)
{
	float	a[3];
	float	phi;
	float	p1[3], p2[3], d[3];
	float	t;

	if (p1x == p2x && p1y == p2y)
	{
		q[0] = 0.0;
		q[1] = 0.0;
		q[2] = 0.0;
		q[3] = 1.0;
		return;
	}

	vset(p1, p1x, p1y, tb_project_to_sphere(TRACKBALLSIZE, p1x, p1y));
	vset(p2, p2x, p2y, tb_project_to_sphere(TRACKBALLSIZE, p2x, p2y));
	vcross(p2, p1, a);
	vsub(p1, p2, d);
	t = vlength(d) / (2.0 * TRACKBALLSIZE);

	if (t > 1.0) t = 1.0;
	if (t < -1.0) t = -1.0;
	phi = 2.0 * asin(t);

	axis_to_quat(a, phi, q);
}