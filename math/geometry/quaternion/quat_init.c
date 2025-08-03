/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quat_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 15:37:19 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/03 15:57:20 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quaternion.h"

/*
** Initializes a quaternion with the given components (x, y, z, w).
** Usage: quat_init(&q, x, y, z, w);
** Expects: q is a valid pointer.
*/
void	quat_init(t_quat *q, float x, float y, float z, float w)
{
	q->x = x;
	q->y = y;
	q->z = z;
	q->w = w;
}
