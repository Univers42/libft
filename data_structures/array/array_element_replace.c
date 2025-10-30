/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_element_replace.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 00:22:23 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/30 21:02:46 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "array.h"

void array_element_replace(t_arr_elem *ae, char *v)
{
	if (ae)
	{
		free(ae->val);
		ae->val = v;
	}
}