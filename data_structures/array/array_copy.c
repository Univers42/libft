/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_copy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 00:07:54 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/30 00:11:44 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "array.h"

t_array	*array_copy(t_array *a)
{
	t_array	*cpy;
	t_arr_elem		*ae;
	t_arr_elem		*new;

	if (a == 0)
		return ((t_array *)NULL);
	cpy = array_create();
	cpy->max_idx = a->max_idx;
	cpy->nelem = a->nelem;
	ae = element_forw(a->head);
	while (ae != a->head)
	{
		new = array_create_element(element_index(ae), element_value(ae));
		add_before(cpy->head, new);
		if (ae == lastref(a))
			set_lastref(cpy, new);
		ae = element_forw(ae);
	}
	return (cpy);
}