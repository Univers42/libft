/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dyn_str_pushstr.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:36:35 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/06 18:44:30 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dyn_string.h"

int dyn_str_pushstr(t_dyn_str *v, const char *str)
{
	int i;

	i = -1;
	while (str[++i])
		if (dyn_str_push(v, str[i]))
			return (0);
	return (1);
}
