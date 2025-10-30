/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_copy_element.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 00:03:18 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/30 21:05:09 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "array.h"
#include "ft_string.h"

t_arr_elem *array_copy_element(t_arr_elem *ae)
{
	if (ae)
		return (array_create_element(element_index(ae),
									element_value(ae) ? ft_strdup(element_value(ae)) : NULL));
	return (NULL);
}