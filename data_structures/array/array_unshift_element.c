/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_unshift_element.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 15:39:48 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/30 15:41:09 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "array.h"

t_arr_elem	*array_unshift_element(t_array *a)
{
	return (array_shift(a, 1, 0));
}