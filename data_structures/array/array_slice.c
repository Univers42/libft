/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_slice.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 15:15:49 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/30 15:20:52 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "array.h"

t_array *array_slice(t_array *array, t_arr_elem *s, t_arr_elem *e)
{
	t_array *a;
	t_arr_elem  *p;
	t_arr_elem  *n;
	size_t i;
	size_t  mi;

	a = array_create();
	mi = 0;
	p = s;
	i = 0;
	while (p != e)
	{
		n = array_create_element(element_index(p), element_value(p));
		add_before(a->head, n);
		mi = element_index(n);
		p = element_forward(p);
		i++;
	}
	a->nelem = i;
	a->max_idx = mi;
	return (a);
}