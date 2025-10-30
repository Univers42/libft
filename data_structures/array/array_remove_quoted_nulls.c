/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_remove_quoted_nulls.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 15:42:36 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/30 15:45:06 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "array.h"

t_array *array_remove_quoted_nulls(t_array *array)
{
    t_arr_elem  *a;

    if (array == 0 || array_head(array) == 0 || array_empty(array))
        return ((t_array *)NULL);
    a = element_forw(array->head);
    while (a != array->head)
    {
        a = element_forw(a);
        a->val = remove_quoted_nulls(a->val);
    }
    return (array);
}