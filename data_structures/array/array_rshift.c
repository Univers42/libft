/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_rshift.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 15:36:09 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/30 21:03:56 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "array.h"

static void renumber_indices(t_arr *a)
{
	t_arr_elem *cur;
	size_t idx = 0;

	if (!a)
		return;
	for (cur = element_forw(a->head); cur != a->head; cur = element_forw(cur))
		cur->ind = idx++;
	a->nelem = idx;
	a->first_idx = (idx > 0) ? element_index(a->head->next) : 0;
	a->max_idx = (idx > 0) ? element_index(a->head->prev) : (size_t)-1;
	invalidate_lastref(a);
}

int array_rshift(t_arr *a, int n, char *s)
{
	t_arr_elem *ae;
	t_arr_elem *new;

	if (a == 0 || (array_empty(a) && s == 0))
		return (0);
	else if (n <= 0)
		return (a->nelem);
	ae = element_forw(a->head);
	if (s)
	{
		/* insert new element at front (logical push) */
		new = array_create_element(0, s);
		add_before(ae, new);
		/* renumber to rebuild contiguous indices */
		renumber_indices(a);
		return (a->nelem);
	}
	/* if no string provided, just shift indices by n -> but renumbering
	   keeps indices contiguous and simpler */
	renumber_indices(a);
	return (a->nelem);
}