/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_state_small.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 16:06:26 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/29 13:42:35 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private_var.h"
#include "var.h"

#ifdef SMALL

int	add_term_histsize(t_var_state *state, int i)
{
	return (i);
}
#else

int	add_term_histsize(t_var_state *state, int i)
{
	state->varinit[i++] = make_term();
	state->varinit[i++] = make_histsize();
	return (i);
}
#endif
