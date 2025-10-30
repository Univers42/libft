/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dyn_str_pushnstr.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:36:33 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/30 23:42:48 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "dyn_string.h"

int	dyn_str_pushnstr(t_dyn_str *v, char *str, int len)
{
	int	i;

	i = -1;
	while (++i < len && str[i])
		if (dyn_str_push(v, str[i]))
			return (0);
	return (1);
}