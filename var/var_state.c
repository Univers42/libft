/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_state.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 22:02:34 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/11 22:03:05 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
        /* Zero/initialize fields that may otherwise contain indeterminate
           data when the struct is not fully explicitly initialized. */
        ft_memcpy(state.linenovar, "LINENO=", 7);
        state.linenovar[7] = '\0';

        /* Ensure option arrays and oplist/oplist are zeroed so reads such
           as get_optlist()[AFLAG] are defined. */
        ft_memset(state.optlist, 0, NOPTS);
        ft_memset(state.oplist, 0, NOPTS);

        state.localvar_stack = NULL;
        state.vlineno_ptr = NULL;
        state.lineno = 0;

        state.defpathvar = DEFPATHVAR;
        state.defifsvar = DEFIFSVAR;
        state.defoptindvar = DEFOPTINDVAR;
        init_varinit_array(&state);
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

t_var make_ps1(void)
{
    return (t_var){NULL, VSTR_FIXED | VTEXT_FIXED, "PS1=$ ", 0};
}

t_var make_ps2(void)
{
    return (t_var){NULL, VSTR_FIXED | VTEXT_FIXED, "PS2=> ", 0};
}
