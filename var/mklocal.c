/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mklocal.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 16:09:55 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/01 15:30:14 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private_var.h"
#include <string.h>

static char *snapshot_opt(t_localvar *lvp);
static void update_save_previous_state(t_localvar *lvp, char *name, int flags);

void mklocal(char *name, int flags)
{
	t_localvar *lvp;
	t_var *vp;
	char *p;
	t_var_state *state;
	t_localvar_list *frame;

	intoff();
	state = get_var_state();
	frame = state->localvar_stack;
	if (frame == NULL)
	{
		/* Lazily create a first localvar frame if none exists yet */
		frame = xmalloc(sizeof(*frame));
		frame->lv = NULL;
		frame->next = NULL;
		state->localvar_stack = frame;
	}
	lvp = xmalloc(sizeof(*lvp));
	/* Default initialize fields */
	lvp->vp = NULL;
	lvp->text = NULL;
	lvp->flags = 0;
	lvp->restore = NULL;

	/* Branch explicitly; avoid ternary with uninitialized vp */
	if (name[0] == '-' && name[1] == '\0')
	{
		p = snapshot_opt(lvp);
		(void)p;
		vp = NULL;
	}
	else
	{
		update_save_previous_state(lvp, name, flags);
		vp = lvp->vp;
		(void)vp;
	}

	/* Push into current localvar stack frame */
	lvp->next = frame->lv;
	frame->lv = lvp;
	inton();
}

/* Snapshot current option list into a newly allocated buffer */
static char *snapshot_opt(t_localvar *lvp)
{
	char *opt_list;
	size_t len;
	char *p;

	opt_list = get_optlist();
	if (!opt_list)
	{
		lvp->text = NULL;
		return (NULL);
	}
	len = ft_strlen(opt_list);
	p = xmalloc(len + 1);
	ft_memcpy(p, opt_list, len + 1);
	lvp->text = p;
	lvp->flags = 0;
	lvp->vp = NULL;
	return (p);
}

/* Save previous state for the named variable or create a placeholder */
static void update_save_previous_state(t_localvar *lvp, char *name, int flags)
{
	char *eq;
	t_var *vp;

	vp = *find_var(name);
	eq = ft_strchr(name, '=');
	if (vp == NULL)
	{
		/* variable does not exist: create it */
		if (eq)
			vp = set_vareq(name, VSTR_FIXED | flags);
		else
			vp = set_var(name, NULL, VSTR_FIXED | flags);
		lvp->flags = VUNSET;
		lvp->text = NULL;
	}
	else
	{
		/* snapshot current state */
		lvp->text = vp->text;
		lvp->flags = vp->flags;
		/* pin storage while local override is active */
		vp->flags |= VSTR_FIXED | VTEXT_FIXED;
		if (eq)
			set_vareq(name, flags);
	}
	lvp->vp = vp;
}
