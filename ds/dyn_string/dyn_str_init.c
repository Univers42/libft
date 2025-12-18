/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dyn_str_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:36:27 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/18 21:28:16 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dyn_string.h"

t_dyn_str	*get_dyn_str(t_dyn_str *maybe)
{
	static	t_dyn_str  str	= {0};

	if (maybe)
		return (maybe);
	return (&str);
}

int	dyn_str_init(t_dyn_str *ret)
{
	if (ret)
	{
		*ret = (t_dyn_str){0};
		get_dyn_str(ret);
	}
	else
		get_dyn_str(NULL);
	return (0);
}
