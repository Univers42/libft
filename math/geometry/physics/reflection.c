/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reflection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 20:34:14 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/26 20:39:31 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "matrices.h"

/**
 * Vector reflexion
 * computes the reflexion of vector `v` about the normal vector `norm`
 * used in physics and graphics for reflecting direction
 * (e.g.m light, velocity).
 */
t_fvec3	reflect(t_fvec3 v, t_fvec3 norm)
{
	return (fvec3_sub(v,
			fvec3_scale(norm,
				2 * fvec3_dot(norm, v))));
}
