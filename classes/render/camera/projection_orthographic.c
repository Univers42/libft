/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projection_orthographic.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 14:39:05 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/03 14:41:53 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "camera.h"

void	orto_projection(t_point *points, t_point *projection, int len)
{
	int		i;
	float	matrix[3][3];

	matrix[0][0] = 1;
	matrix[1][1] = 1;
	i = 0;
	while (i < len)
	{
		projection[i] = mul_mat(project_matrix, points[i]);
		i++;
	}
}