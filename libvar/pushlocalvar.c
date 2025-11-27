/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pushlocalvar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 16:09:43 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/27 16:41:44 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "var.h"

t_localvar_list	*pushlocalvars(int push)
{
	t_localvar_list	*ll;
	t_localvar_list	*top;
	t_var_state		*state;

	state = get_var_state();
	top = state->localvar_stack;
	if (push)
	{
		intoff();
		ll = ckmalloc(sizeof(*ll));
		ll->lv = NULL;
		ll->next = top;
		state->localvar_stack = ll;
		inton();
	}
	return (top);
}
