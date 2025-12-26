/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shapes1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 21:29:51 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/26 21:32:47 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "algebra.h"
#include "matrices.h"
#include "geometry.h"

t_fvec3	cylinder_norm(t_cylinder cy, t_fvec3 pos, t_collision coll)
{
	float	t;

	if (coll.disk_collision)
		return (cy.dir);
	t = fvec3_dot(cy.dir, fvec3_sub(pos, cy.b));
	return (fvec3_norm(fvec3_sub(fvec3_sub(pos, fvec3_scale(cy.dir, t)),
			cy.b)));
}
