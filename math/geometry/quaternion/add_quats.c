/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_quats.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 13:58:41 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/04 14:27:08 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quaternion.h"

void	add_quats(float q1[4], float q2[4], float dest[4])
{
	static int count=0;
	float t1[4], t2[4], t3[4];
	float tf[4];

#if 0
printf("q1 = %f %f %f %f\n", q1[0], q1[1], q1[2], q1[3]);
printf("q2 = %f %f %f %f\n", q2[0], q2[1], q2[2], q2[3]);
#endif

	vcopy(q1,t1);
	vscale(t1,q2[3]);

	vcopy(q2,t2);
	vscale(t2,q1[3]);

	vcross(q2,q1,t3);
	vadd(t1,t2,tf);
	vadd(t3,tf,tf);
	tf[3] = q1[3] * q2[3] - vdot(q1,q2);

#if 0
printf("tf = %f %f %f %f\n", tf[0], tf[1], tf[2], tf[3]);
#endif

	dest[0] = tf[0];
	dest[1] = tf[1];
	dest[2] = tf[2];
	dest[3] = tf[3];

	if (++count > RENORMCOUNT) {
		count = 0;
		normalize_quat(dest);
	}
}