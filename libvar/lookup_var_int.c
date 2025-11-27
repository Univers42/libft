/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lookup_var_int.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 16:09:58 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/27 16:47:03 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "var.h"

intmax_t	lookupvarint(const char *name)
{
	char	*value;

	value = lookupvar(name);
	if (value == NULL)
		return (ft_atomax("", 0));
	return (ft_atomax(value, 0));
}
