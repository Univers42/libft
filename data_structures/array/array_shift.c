/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_shift.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 15:21:00 by dlesieur          #+#    #+#             */
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

t_arr_elem *array_shift(t_arr *a, int n, int flags)
{
	t_arr_elem *ae;
	t_arr_elem *ret;
	int i;

	if (a == 0 || array_empty(a) || n <= 0)
		return ((t_arr_elem *)NULL);
	invalidate_lastref(a);
	i = 0;
	ret = element_forw(a->head);
	ae = element_forw(a->head);
	while (ae != a->head && i < n)
	{
		ae = element_forw(ae);
		i++;
	}
	if (ae == a->head)
	{
		if (flags & AS_DISPOSE)
		{
			array_flush(a);
			return (t_arr_elem *)NULL;
		}
		/* removing all elements but returning the old list */
		ae = ret;
		while (element_forw(ae) != a->head)
			ae = element_forw(ae);
		/* terminate the returned list so callers won't traverse into head */
		ae->next = (t_arr_elem *)NULL;
		a->head->next = a->head;
		a->head->prev = a->head;
		a->max_idx = (size_t)-1;
		a->nelem = 0;
		set_first_index(a, 0);
		return (ret);
	}
	/* detach first n elements: ae is the element after removed block */
	ae->prev->next = (t_arr_elem *)NULL;
	a->head->next = ae;
	ae->prev = a->head;
	/* renumber remaining elements */
	renumber_indices(a);

	if (flags & AS_DISPOSE)
	{
		ae = ret;
		while (ae)
		{
			ret = element_forw(ae);
			array_dispose_element(ae);
			ae = ret;
		}
		return ((t_arr_elem *)NULL);
	}
	return (ret);
}
