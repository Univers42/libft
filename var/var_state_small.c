/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_state_small.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 23:44:39 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/11 22:03:06 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "var.h"

#ifdef SMALL

int add_term_histsize(t_var_state *state, int i)
{
	return (i);
}
#else

int add_term_histsize(t_var_state *state, int i)
{
	state->varinit[i++] = make_term();
	state->varinit[i++] = make_histsize();
	return (i);
}
#endif
