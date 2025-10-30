/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_remove.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 15:47:45 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/30 21:03:56 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "array.h"

/*
 * Delete the element with index i from array a and return it so the
 * caller can dispose of it.
 */
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

t_arr_elem *array_remove(t_arr *a, size_t i)
{
    t_arr_elem *ae;
    t_arr_elem *start;
    size_t startind;
    int direction;

    if (a == NULL || array_empty(a))
        return (NULL);
    if (i > array_max_index(a) || i < array_first_index(a))
        return (NULL);

    start = lastref(a);
    startind = element_index(start);
    if (i < startind / 2)
    {
        start = element_forw(a->head);
        startind = element_index(start);
        direction = 1;
    }
    else if (i >= startind)
        direction = 1;
    else
        direction = -1;

    ae = start;
    while (ae != a->head)
    {
        if (element_index(ae) == i)
        {
            /* unlink */
            ae->next->prev = ae->prev;
            ae->prev->next = ae->next;
            a->nelem--;
            /* renumber remaining elements to keep indices contiguous */
            renumber_indices(a);

            /* update lastref */
            if (ae->next != a->head)
                set_lastref(a, ae->next);
            else if (ae->prev != a->head)
                set_lastref(a, ae->prev);
            else
                invalidate_lastref(a);

            return (ae);
        }
        if (direction == 1)
            ae = element_forw(ae);
        else
            ae = element_back(ae);
        if (direction == 1 && element_index(ae) > i)
            break;
        if (direction == -1 && element_index(ae) < i)
            break;
    }
    return (NULL);
}