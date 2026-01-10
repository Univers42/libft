/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lerp_point.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 15:00:21 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/04 23:54:58 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pixel.h"
#include "algebra.h"

// Interpolate between two points
void	lerp_point(t_point *result, const t_point *a, const t_point *b, float t)
{
	result->axis[X] = ft_lerp(a->axis[X], b->axis[X], t);
	result->axis[Y] = ft_lerp(a->axis[Y], b->axis[Y], t);
	result->axis[Z] = ft_lerp(a->axis[Z], b->axis[Z], t);
	result->color = a->color;
	result->color.hex_color = a->color.hex_color;
	result->paint = a->paint;
	result->polar[LAT] = ft_lerp(a->polar[LAT], b->polar[LAT], t);
	result->polar[LONG] = ft_lerp(a->polar[LONG], b->polar[LONG], t);
}
