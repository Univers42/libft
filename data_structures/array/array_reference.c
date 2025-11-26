/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_reference.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 14:38:34 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/26 13:53:23 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "array.h"

static void	choose_start_and_direction(t_arr *a,
										size_t i,
										t_arr_elem **start,
										int *direction)
{
	size_t	startind;

	*start = lastref(a);
	startind = element_index(*start);
	if (i < startind / 2)
	{
		*start = element_forw(a->head);
		*direction = 1;
	}
	else if (i >= startind)
		*direction = 1;
	else
		*direction = -1;
}

static char	*walk_and_find(t_arr *a, size_t i, t_arr_elem *start, int dir)
{
	t_arr_elem	*ae;

	ae = start;
	while (ae != a->head)
	{
		if (element_index(ae) == i)
		{
			set_lastref(a, ae);
			return (element_value(ae));
		}
		if (dir == 1)
			ae = element_forw(ae);
		else
			ae = element_back(ae);
		if (dir == 1 && element_index(ae) > i)
			break ;
		if (dir == -1 && element_index(ae) < i)
			break ;
	}
	return (NULL);
}

char	*array_reference(t_arr *a, size_t i)
{
	t_arr_elem	*start;
	int			direction;
	char		*res;

	if (a == NULL || array_empty(a))
		return (NULL);
	if (i > array_max_index(a) || i < array_first_index(a))
		return (NULL);
	choose_start_and_direction(a, i, &start, &direction);
	res = walk_and_find(a, i, start, direction);
	if (res == NULL)
		set_lastref(a, start);
	return (res);
}
