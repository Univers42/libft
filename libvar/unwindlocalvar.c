/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unwindlocalvar.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 16:06:55 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/27 16:42:28 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "var.h"

void	unwindlocalvars(t_localvar_list *stop)
{
	t_var_state	*state;

	state = get_var_state();
	while (state->localvar_stack != stop)
		poplocalvars();
}
