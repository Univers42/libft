/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_find.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 16:33:02 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/27 16:33:14 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "var.h"

struct s_var	**var_find(struct s_var **vpp, const char *name)
{
	while (*vpp)
	{
		if (libvar_varcmp((*vpp)->text, name) == 0)
			break ;
		vpp = &(*vpp)->next;
	}
	return (vpp);
}
