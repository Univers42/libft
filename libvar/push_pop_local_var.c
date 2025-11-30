/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_pop_local_var.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 16:09:53 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/29 17:26:50 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private_var.h"
#include "var.h"

static void	process_local_var_list(t_localvar *lvp_list,
								t_var_state *state);
static void	restore_opt_var(t_localvar *lvp, t_var_state *state);

// Public API
void	pop_local_vars(void)
{
    t_localvar_list	*ll;
    t_var_state		*state;
    t_localvar		*lvp_list;

    state = get_var_state();
    intoff();
	ll = state->localvar_stack;
	if (ll == NULL)
	{
		inton();
		return;
	}
	state->localvar_stack = ll->next;
    lvp_list = ll->lv;
    xfree(ll);
    process_local_var_list(lvp_list, state);
	inton();
}

t_localvar_list	*push_local_vars(int push)
{
	t_localvar_list	*ll;
	t_localvar_list	*top;
	t_var_state		*state;

	state = get_var_state();
	top = state->localvar_stack;
	if (push)
	{
		intoff();
		ll = xmalloc(sizeof(*ll));
		ll->lv = NULL;
		ll->next = top;
		state->localvar_stack = ll;
		inton();
	}
	return (top);
}

// PRIVATE HELPERS

static void	restore_regular_var(t_localvar *lvp)
{
    t_var   *vp;

    vp = lvp->vp;
    if (lvp->flags == VUNSET)
    {
        vp->flags &= ~(VSTR_FIXED | VREAD_ONLY);
        unset_var(vp->text);
    }
    else
    {
        if (vp->func)
            (*vp->func)(ft_strchrnul(lvp->text, '=') + 1);
        if ((vp->flags & (VTEXT_FIXED | VSTACK)) == 0)
            xfree((void *)vp->text);
        vp->flags = lvp->flags;
        vp->text = lvp->text;
    }
}

/**
 * For a generci library t's better to avid sentinel hacks and
 * provide an explicit per-entry restore callback for the shell adapter
 * can set when it creates a special entry
 * speaking of current->restore()
 */
static void	process_local_var_list(t_localvar *lvp_list,
								t_var_state *state)
{
	t_localvar	*current;
	t_localvar	*next;

	current = lvp_list;
	while (current != NULL)
	{
		next = current->next;
		if (current->restore)
			current->restore(current, state);
		else
			restore_regular_var(current);
		xfree(current);
		current = next;
	}
}
