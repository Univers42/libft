/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_state_with_lineno.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 22:00:59 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/11 22:03:06 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "var.h"

#ifdef WITH_LINENO

static void getoptsreset(char *unused)
{
	(void)unused;
	return;
}

int init_varinit_pt2(t_var_state *state, int i)
{
	state->varinit[i++] = make_ps1();
	state->varinit[i++] = make_ps2();
	state->varinit[i++] = make_ps4();
	/* cast callback to the prototype expected by make_optind to avoid
	   incompatible-pointer-type diagnostics across translation units */
	state->varinit[i++] = make_optind(state->defoptindvar, (void (*)(void))getoptsreset);
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
	state->varinit[i++] = make_optind(state->defoptindvar, (void (*)(void))getoptsreset);
	i = add_term_histsize(state, i);
	return (i);
}

#endif
