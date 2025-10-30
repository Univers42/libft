/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_remove.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 15:47:45 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/30 15:47:51 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "array.h"

/*
 * Delete the element with index I from array A and return it so the
 * caller can dispose of it.
 */
ARRAY_ELEMENT *
array_remove(ARRAY *a, arrayind_t i)
{
	register ARRAY_ELEMENT *ae, *start;
	arrayind_t startind;
	int direction;

	if (a == 0 || array_empty(a))
		return((ARRAY_ELEMENT *) NULL);
	if (i > array_max_index(a) || i < array_first_index(a))
		return((ARRAY_ELEMENT *)NULL);	/* Keep roving pointer into array to optimize sequential access */
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
	for (ae = start; ae != a->head; ) {
		if (element_index(ae) == i) {
			ae->next->prev = ae->prev;
			ae->prev->next = ae->next;
			a->num_elements--;
			if (i == array_max_index(a))
				a->max_index = element_index(ae->prev);
#if 0
			INVALIDATE_LASTREF(a);
#else
			if (ae->next != a->head)
				SET_LASTREF(a, ae->next);
			else if (ae->prev != a->head)
				SET_LASTREF(a, ae->prev);
			else
				INVALIDATE_LASTREF(a);
#endif
			return(ae);
		}
		ae = (direction == 1) ? element_forw(ae) : element_back(ae);
		if (direction == 1 && element_index(ae) > i)
			break;
		else if (direction == -1 && element_index(ae) < i)
			break;
	}
	return((ARRAY_ELEMENT *) NULL);
}
