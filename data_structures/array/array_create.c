/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_create.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 23:51:17 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/30 16:25:43 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "array.h"

t_array *array_create(void)
{
    t_array *r;
    t_arr_elem *head;

    r = (t_array *)malloc(sizeof(t_array));
    if (!r)
        return (NULL);
    r->max_idx = (size_t)-1;
    r->nelem = 0;
    r->lastref = (t_arr_elem *)0;
    head = array_create_element((size_t)-1, (char *)NULL);
    if (!head)
    {
        free(r);
        return (NULL);
    }
    head->next = head;
    head->prev = head;
    r->head = head;
    r->first_idx = 0;
    r->alloc_size = 0;
    r->elements = NULL;
    return (r);
}
