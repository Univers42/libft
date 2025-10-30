/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_insert.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 01:05:45 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/30 01:08:35 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "array.h"
/**
 * add a new element with index I and value V to array A (a[i] = v).
 */
int	array_insert(t_array *a, size_t i, char *v)
{
	t_arr_elem	*new;
	t_arr_elem	*ae;
	t_arr_elem	*start;
	size_t		startind;
	int			direction;

	if (a == 0)
		return(-1);
	new = array_create_element(i, v);
	if (i > array_max_index(a))
	{
		/*
		 * Hook onto the end.  This also works for an empty array.
		 * Fast path for the common case of allocating arrays
		 * sequentially.
		 */
		ADD_BEFORE(a->head, new);
		a->max_index = i;
		a->num_elements++;
		SET_LASTREF(a, new);
		return(0);
	} else if (i < array_first_index(a)) {
		/* Hook at the beginning */
		ADD_AFTER(a->head, new);
		a->num_elements++;
		SET_LASTREF(a, new);
		return(0);
	}
#if OPTIMIZE_SEQUENTIAL_ARRAY_ASSIGNMENT
	/*
	 * Otherwise we search for the spot to insert it.  The lastref
	 * handle optimizes the case of sequential or almost-sequential
	 * assignments that are not at the end of the array.
	 */
	start = LASTREF(a);
	/* Use same strategy as array_reference to avoid paying large penalty
	   for semi-random assignment pattern. */
	startind = element_index(start);
	if (i < startind/2) {
		start = element_forw(a->head);
		startind = element_index(start);
		direction = 1;
	} else if (i >= startind) {
		direction = 1;
	} else {
		direction = -1;
	}
#else
	start = element_forw(ae->head);
	startind = element_index(start);
	direction = 1;
#endif
	for (ae = start; ae != a->head; ) {
		if (element_index(ae) == i) {
			/*
			 * Replacing an existing element.
			 */
			free(element_value(ae));
			/* Just swap in the new value */
			ae->value = new->value;
			new->value = 0;
			array_dispose_element(new);
			SET_LASTREF(a, ae);
			return(0);
		} else if (direction == 1 && element_index(ae) > i) {
			ADD_BEFORE(ae, new);
			a->num_elements++;
			SET_LASTREF(a, new);
			return(0);
		} else if (direction == -1 && element_index(ae) < i) {
			ADD_AFTER(ae, new);
			a->num_elements++;
			SET_LASTREF(a, new);
			return(0);
		}
		ae = direction == 1 ? element_forw(ae) : element_back(ae);
	}
	array_dispose_element(new);
	INVALIDATE_LASTREF(a);
	return (-1);		/* problem */
}