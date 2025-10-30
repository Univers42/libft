/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 14:35:37 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/30 14:37:11 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "array.h"

t_array *array_quote(t_array *array)
{
	t_arr_elem  *ae;
	char        *t;

	if (array == 0 || array_head(array) == 0 || array_empty(array))
		return (t_array *)NULL;
    ae = element_forw(array->head);
	while (ae != array->head)
    {
		t = quote_string (ae->val);
		free(ae->val);
		ae->val = t;
        ae = element_forw(ae);
	}
	return (array);
}