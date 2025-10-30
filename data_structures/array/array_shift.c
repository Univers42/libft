/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_shift.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 15:21:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/30 15:35:55 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "array.h"

t_arr_elem	*array_shift(t_array *a, int n, int flags)
{
	t_arr_elem	*ae;
	t_arr_elem	*ret;
	t_arr_elem	*p;
	size_t		elem;
	int			i;

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
			return (array_flush(a), (t_arr_elem *)NULL);
		ae = ret;
		while (element_forw(ae) != a->head)
			ae = element_forw(ae);
		p = element_forw(ae);
		p = (t_arr_elem *)NULL;
		a->head->next = a->head;
		a->head->prev = a->head;
		a->max_idx = -1;
		a->nelem = 0;
		return (ret);
	}
	ae->prev->next = (t_arr_elem *)NULL;
	a->head->next = ae;
	ae->prev = a->head;
	while(ae != a->head)
	{
		elem = element_index(ae);
		elem -= n;
		ae = element_forw(ae);
	}
	a->nelem -= n;
	a->max_idx = element_index(a->head->prev);
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
