/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quat_normalize.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 15:37:27 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/03 15:57:21 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quaternion.h"

/*
** Normalizes the quaternion to unit length.
** Usage: quat_normalize(&q);
** Expects: q is a valid pointer.
*/
void	quat_normalize(t_quat *q)
{
	float	mag;

	mag = sqrtf(q->x * q->x + q->y * q->y + q->z * q->z + q->w * q->w);
	if (mag == 0.0f)
		return ;
	q->x /= mag;
	q->y /= mag;
	q->z /= mag;
	q->w /= mag;
}
