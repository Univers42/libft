/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_vareq.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 16:09:37 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/01 01:08:16 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private_var.h"
#include "var.h"

static t_var *variable_not_found(char *s, int *pflags, t_var **vpp);
static t_var *variable_exist(char *s, int *pflags, t_var *vp, t_var **vpp);

t_var *set_vareq(char *s, int flags)
{
	t_var *vp;
	t_var **vpp;
	t_var *ret;

	/* preserve existing semantics, ensure AFLAG is defined in private_var.h */
	flags |= (VEXPORT & ((unsigned)(1 - get_optlist()[AFLAG])) - 1);
	vpp = find_var(s);
	vp = *vpp;
	if (vp)
		variable_exist(s, &flags, vp, vpp);
	else
		variable_not_found(s, &flags, vpp);
	if (ret != NULL || (vp == NULL && ret == NULL && *vpp == NULL))
		return (ret);
	if (!(flags & (VTEXT_FIXED | VSTACK | VNO_SAVE)))
		s = save_str(s);
	if (vp == NULL)
		vp = *vpp;
	vp->text = s;
	vp->flags = flags;
	if (vp->func && (flags & VNO_FUNC) == 0)
		(*vp->func)(var_null(s));
	return (vp);
}

static t_var *variable_exist(char *s, int *pflags, t_var *vp, t_var **vpp)
{
	unsigned bits;
	const char *n;
	int flags;

	flags = *pflags;
	if (vp->flags & VREAD_ONLY)
	{
		if (flags & VNO_SAVE)
			free(s);
		n = vp->text;
		/* compute length safely: pointer subtraction of same pointer types */
		sh_error("%.*s: is read only", (int)(ft_strchrnul(n, '=') - n));
	}
	if ((vp->flags) & (VTEXT_FIXED | VSTACK) == 0)
		xfree((void *)vp->text);
	if ((flags & (VEXPORT | VREAD_ONLY | VSTR_FIXED | VUNSET)) != VUNSET)
	{
		bits = ~(VTEXT_FIXED | VSTACK | VNO_SAVE | VUNSET);
		flags |= vp->flags & bits;
		*pflags = flags;
		return (NULL);
	}
	if ((vp->flags & VSTR_FIXED))
	{
		flags |= VSTR_FIXED;
		*pflags = flags;
		return (NULL);
	}
	*vpp = vp->next;
	xfree(vp);
	if ((flags & (VTEXT_FIXED | VSTACK | VNO_SAVE)) == VNO_SAVE)
		xfree(s);
	return (vp);
}

static t_var *variable_not_found(char *s, int *pflags, t_var **vpp)
{
	int flags;
	t_var *vp;

	flags = *pflags;
	if ((flags & (VEXPORT | VREAD_ONLY | VSTR_FIXED | VUNSET)) == VUNSET)
	{
		if ((flags & (VTEXT_FIXED | VSTACK | VNO_SAVE)) == VNO_SAVE)
			xfree(s);
		return (NULL);
	}
	vp = xmalloc(sizeof(*vp));
	vp->next = *vpp;
	vp->func = NULL;
	*vpp = vp;
	return (vp);
}