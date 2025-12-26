/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   points.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 23:28:49 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/26 23:28:56 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "geometry.h"

t_triangle_pts	triangle_points(t_vec meshes, t_triangle triangle)
{
	t_mesh			*mesh;
	int				*vidxs;
	t_triangle_pts	ret;

	mesh = meshes.buff[triangle.mesh_idx];
	vidxs = vertex_idxs.buff[triangle.triangle_idx * 3];
	ret.a = mesh->vertices.buff[vidxs[0]];
	ret.b = mesh->vertices.buff[vidxs[1]];
	ret.c = mesh->vertices.buff[vidxs[2]];
	return  (ret);
}

