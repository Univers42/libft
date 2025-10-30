/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lastref_start.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 15:52:02 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/30 16:11:40 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "array.h"

t_arr_elem	*lastref_start(t_array *a, size_t i)
{
	if (is_lastref(a) && i >= element_index(a->lastref))
		return (a->lastref);
	return (element_forw(a->head));
}