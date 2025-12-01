/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   accessors.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 15:37:04 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/01 00:49:09 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "var.h"
#include "private_var.h"

char *get_optlist(void)
{
	t_var_state *state;

	state = get_var_state();
	if (!state)
		return (NULL);
	/* The var_state owns the canonical option list storage (oplist). */
	return (state->oplist);
}

t_localvar_list *get_localvar_stack(void)
{
	t_var_state *state;

	state = get_var_state();
	if (!state)
		return (NULL);
	/* Return the pointer to the current localvar_stack managed in state. */
	return (state->localvar_stack);
}
