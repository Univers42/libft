/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_dispose.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 00:21:38 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/30 21:03:58 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "array.h"

void array_dispose(t_arr *a)
{
	if (a == 0)
		return;
	array_flush(a);
	array_dispose_element(a->head);
	free(a);
}