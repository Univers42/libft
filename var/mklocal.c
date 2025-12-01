/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mklocal.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 16:09:55 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/29 15:55:33 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private_var.h"
#include <string.h>

static char		*snapshot_opt(t_localvar *lvp, t_var *vp);
static void		*update_save_previous_state(t_localvar *lvp, t_var *vp,
					char *name, int flags);

void	mklocal(char *name, int flags)
{
	t_localvar	*lvp;
	t_var		*vp;
	char		*p;

	intoff();
	lvp = xmalloc(sizeof(t_localvar));
	ternary(name[0] == '-' && name[1] == '\0',
		snapshot_opt(lvp, vp),
		update_save_previous_state(lvp, vp, name, flags));
	lvp->vp = vp;
	lvp->next = get_localvar_stack()->lv;
	get_localvar_stack()->lv = lvp;
	inton();
}

static char	*snapshot_opt(t_localvar *lvp, t_var *vp)
{
	char	*p;
	char	*opt_list;

	opt_list = get_optlist();
	p = xmalloc(sizeof(opt_list));
	lvp->text = ft_memcpy(p, opt_list, sizeof(opt_list));
	vp = NULL;
	return (p);
}

static void	*update_save_previous_state(t_localvar *lvp, t_var *vp,
				char *name, int flags)
{
	char	*eq;

	vp = *find_var(name);
	eq = ft_strchr(name, '=');
	if (vp == NULL)
	{
		vp = ternary(vp,
				set_vareq(name, VSTR_FIXED | flags),
				set_var(name, NULL, VSTR_FIXED | flags));
		lvp->flags = VUNSET;
	}
	else
	{
		lvp->text = vp->text;
		lvp->flags = vp->flags;
		vp->flags |= VSTR_FIXED | VTEXT_FIXED;
		if (eq)
			set_vareq(name, flags);
	}
}
