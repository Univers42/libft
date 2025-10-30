/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_dequote.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 00:18:09 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/30 16:28:01 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "array.h"

t_array *array_dequote(t_array *array)
{
	t_arr_elem	*a;

	if (array == 0 || array_head(array) == 0 || array_empty(array))
		return ((t_array *)NULL);
	a = element_forw(array->head);
	while (a != array->head)
	{
		t = dequote_string(a->val);
		free(a->val);
		a = element_forw(a);
	}
	return (array);
}