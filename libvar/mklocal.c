/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mklocal.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 16:09:55 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/28 15:38:04 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private_var.h"
#include <string.h>

static t_localvar	*handle_special_opt(void);
static void			setup_new_local(t_localvar *lvp, char *name, int flags);
static void			setup_existing_local(t_localvar *lvp, t_var *vp,
						char *name, int flags);
static t_localvar	*handle_var_opt(char *name, int flags);

void	mklocal(char *name, int flags)
{
	t_var_state	*state;
	t_localvar	*lvp;

	state = get_var_state();
	intoff();
	if (name[0] == '-' && name[1] == '\0')
		lvp = handle_special_opt();
	else
		lvp = handle_var_opt(name, flags);
	lvp->next = state->localvar_stack->lv;
	state->localvar_stack->lv = lvp;
	inton();
}

static t_localvar	*handle_special_opt(void)
{
	t_var_state			*state;
	t_localvar			*lvp;
	char				*p;

	state = get_var_state();
	lvp = ckmalloc(sizeof(struct s_localvar));
	p = ckmalloc(NOPTS);
	lvp->text = ft_memcpy(p, state->optlist, NOPTS);
	lvp->vp = NULL;
	return (lvp);
}

static void	setup_new_local(t_localvar *lvp, char *name, int flags)
{
	char	*eq;

	eq = ft_strchr(name, '=');
	if (eq)
		lvp->vp = setvareq(name, VSTR_FIXED | flags);
	else
		lvp->vp = setvar(name, NULL, VSTR_FIXED | flags);
	lvp->flags = VUNSET;
}

static void	setup_existing_local(t_localvar *lvp, t_var *vp,
								char *name, int flags)
{
	char	*eq;

	eq = ft_strchr(name, '=');
	lvp->text = vp->text;
	lvp->flags = vp->flags;
	vp->flags |= VSTR_FIXED | VTEXT_FIXED;
	if (eq)
		setvareq(name, flags);
	lvp->vp = vp;
}

static t_localvar	*handle_var_opt(char *name, int flags)
{
	t_localvar	*lvp;
	t_var		*vp;

	lvp = ckmalloc(sizeof(struct s_localvar));
	vp = *var_find(var_hash(name), name);
	if (vp == NULL)
		setup_new_local(lvp, name, flags);
	else
		setup_existing_local(lvp, vp, name, flags);
	return (lvp);
}
