/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_state.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 16:11:49 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/28 18:43:02 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private_var.h"
#include "var.h"

/****************************
 *		FORWARD PRIVATE
 ****************************/
static void init_varinit_array(t_var_state *state);

/****************************
 *		PUBLIC API
 ****************************/

t_var_state *get_var_state(void)
{
    static t_var_state state;
    static int is_initialized = 0;

    if (!is_initialized)
    {
        state.defpathvar = DEFPATHVAR;
        state.defifsvar = DEFIFSVAR;
        state.defoptindvar = DEFOPTINDVAR;
        ft_memcpy(state.linenovar, "LINENO=", 7);
        state.linenovar[7] = '\0';
        init_varinit_array(&state);
        ft_memset(state.oplist, 0, NOPTS);
        is_initialized = 1;
    }
    return (&state);
}

/****************************
 *		PRIVATE HELPERS
 ****************************/

static void init_varinit_array(t_var_state *state)
{
	int i;

	i = 0;
	state->vlineno_ptr = NULL;
	i = add_atty(state, i);
	state->varinit[i++] = make_ifs(state->defifsvar);
	state->varinit[i++] = make_mail();
	state->varinit[i++] = make_mailpath();
	state->varinit[i++] = make_path(state->defpathvar);
	i = init_varinit_pt2(state, i);
	state->varinit_size = i;
}

// --- Local static helpers for this file only ---

t_var	make_ps1(void)
{
	return (t_var){NULL, VSTR_FIXED | VTEXT_FIXED, "PS1=$ ", 0};
}

t_var	make_ps2(void)
{
	return (t_var){NULL, VSTR_FIXED | VTEXT_FIXED, "PS2=> ", 0};
}
