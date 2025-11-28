/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_state_with_lineno.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 16:05:47 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/28 16:37:29 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private_var.h"
#include "var.h"

#ifdef WITH_LINENO

int init_varinit_pt2(t_var_state *state, int i)
{
	state->varinit[i++] = make_ps1();
	state->varinit[i++] = make_ps2();
	state->varinit[i++] = make_ps4();
	state->varinit[i++] = make_optind(state->defoptindvar, getoptsreset);
	state->varinit[i] = make_lineno(state->linenovar);
	state->vlineno_ptr = &state->varinit[i];
	i++;
	i = add_term_histsize(state, i);
	return (i);
}

#else

int init_varinit_pt2(t_var_state *state, int i)
{
	state->varinit[i++] = make_ps1();
	state->varinit[i++] = make_ps2();
	state->varinit[i++] = make_ps4();
	state->varinit[i++] = make_optind(state->defoptindvar, getoptsreset);
	i = add_term_histsize(state, i);
	return (i);
}

#endif
