/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_rshift.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 15:36:09 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/30 15:39:32 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "array.h"

int array_rshift (t_array *a, int n, char *s)
{
	t_arr_elem	*ae;
	t_arr_elem	*new;
	size_t		idx;

	if (a == 0 || (array_empty(a) && s == 0))
		return (0);
	else if (n <= 0)
		return (a->nelem);
	ae = element_forw(a->head);
	if (s)
	{
		new = array_create_element(0, s);
		add_before(ae, new);
		a->nelem++;
		if (array_num_elements(a) == 1)
		{
			a->max_idx = 0;
			return (1);
		}
	}
	while (ae != a->head)
	{
		idx = element_index(ae);
		idx += n;
		ae = element_forw(ae);
	}
	a->max_idx = element_index(a->head->prev);
	invalidate_lastref(a);
	return (a->nelem);
}