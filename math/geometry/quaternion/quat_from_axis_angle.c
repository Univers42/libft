/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quat_from_axis_angle.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 15:37:15 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/03 15:56:59 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quaternion.h"

/*
** Creates a quaternion representing a rotation of angle_rad radians around the axis (axis_x, axis_y, axis_z).
** Usage: quat_from_axis_angle(&q, x, y, z, angle);
** Expects: Non-zero axis vector; angle in radians; q is a valid pointer.
*/
void quat_from_axis_angle(t_quat *q, float axis_x, float axis_y, float axis_z, float angle_rad)
{
    float half;
    float s;
    float mag;

    half = angle_rad * 0.5f;
    s = sinf(half);
    mag = sqrtf(axis_x*axis_x + axis_y*axis_y + axis_z*axis_z);
    if (mag == 0.0f)
    {
        quat_init(q, 0, 0, 0, 1);
        return;
    }
    q->x = axis_x / mag * s;
    q->y = axis_y / mag * s;
    q->z = axis_z / mag * s;
    q->w = cosf(half);
}
