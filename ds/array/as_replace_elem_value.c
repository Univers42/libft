/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   as_replace_elem_value.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 13:35:29 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/26 13:38:13 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "array.h"

/*
replace element value, free old, set lastref
*/
int	as_replace_elem_value(t_arr *a, t_arr_elem *ae, t_arr_elem *new)
{
	char	*old;

	if (!a || !ae || !new)
		return (-1);
	old = ae->val;
	ae->val = new->val;
	new->val = NULL;
	free(old);
	array_dispose_element(new);
	set_lastref(a, ae);
	return (0);
}
