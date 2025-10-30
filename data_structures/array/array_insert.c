/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_insert.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 01:05:45 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/30 21:03:55 by dlesieur         ###   ########.fr       */
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

int array_insert(t_arr *a, size_t i, char *v)
{
    t_arr_elem *new;
    t_arr_elem *ae;
    t_arr_elem *start;
    size_t startind;
    int direction;

    if (a == NULL)
        return (-1);
    new = array_create_element(i, v);
    if (new == NULL)
        return (-1);

    /* Special-case: empty array -> insert as the only element */
    if (array_empty(a))
    {
        add_before(a->head, new);
        /* renumber to make indices contiguous starting at 0 */
        renumber_indices(a);
        set_lastref(a, element_forw(a->head));
        return (0);
    }

    /* Fast path: append at end */
    if (i > array_max_index(a))
    {
        add_before(a->head, new);
        renumber_indices(a);
        set_lastref(a, element_forw(a->head->prev));
        return (0);
    }
    /* Insert before first element */
    if (i < array_first_index(a))
    {
        add_after(a->head, new);
        renumber_indices(a);
        set_lastref(a, element_forw(a->head->next));
        return (0);
    }

    /* Choose start and direction based on lastref optimization */
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
            /* replace existing value */
            char *old = ae->val;
            ae->val = new->val;
            new->val = NULL;
            free(old);
            array_dispose_element(new);
            set_lastref(a, ae);
            return (0);
        }
        if (direction == 1 && element_index(ae) > i)
        {
            add_before(ae, new);
            renumber_indices(a);
            set_lastref(a, element_forw(a->head)); /* lastref invalidated anyway */
            return (0);
        }
        if (direction == -1 && element_index(ae) < i)
        {
            add_after(ae, new);
            renumber_indices(a);
            set_lastref(a, element_forw(a->head)); /* lastref invalidated anyway */
            return (0);
        }
        ae = (direction == 1) ? element_forw(ae) : element_back(ae);
    }

    /* Failed to insert: cleanup and invalidate lastref */
    array_dispose_element(new);
    invalidate_lastref(a);
    return (-1);
}