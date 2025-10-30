/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lastref.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 15:51:31 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/30 16:13:25 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "array.h"

t_arr_elem	*lastref(t_array *a)
{
	if (a->lastref)
		return (a->lastref);
	return (element_forw(a->head));
}