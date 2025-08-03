/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quat_to_matrix.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 15:23:21 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/03 15:57:22 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quaternion.h"

/*
** Converts a quaternion to a 4x4 rotation matrix.
** Usage: quat_to_matrix(&mat, &q);
** Expects: Both pointers are valid and non-NULL.
*/
void quat_to_matrix(t_mat4 *out, const t_quat *q)
{
    matrix_from_quaternion(out, q->x, q->y, q->z, q->w);
}
