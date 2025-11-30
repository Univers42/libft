/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_vareq.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 16:09:37 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/29 18:21:42 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private_var.h"
#include "var.h"

t_var	*set_vareq(char *s, int flags)
{
	t_var	*vp;
	t_var	**vpp;
	

	flags |= (VEXPORT & ((unsigned) (1 - get_optlist()[AFLAG])) - 1);
	vpp = find_var(s);
	vp = *vpp;
	if (vp)
		variable_exist(s, flags, vp, vpp);
	else
		variable_not_found(s, flags, vp, vpp);
	if (!(flags & (VTEXT_FIXED | VSTACK | VNO_SAVE)))
		s = save_str(s);
	vp->text = s;
	vp->flags = flags;
	if (vp->func && (flags & VNO_FUNC) == 0)
		(*vp->func)(var_null(s));
}

t_var	*variable_exist(char *s, int flags, t_var *vp, t_var **vpp)
{
	unsigned	bits;
	const char	*n;

	if (vp->flags & VREAD_ONLY)
	{
		if (flags & VNO_SAVE)
			free(s);
		n = vp->text;
	}
	if ((vp->flags) & (VTEXT_FIXED | VSTACK) == 0)
		xfree(vp->text);
	if ((flags & (VEXPORT | VREAD_ONLY | VSTR_FIXED | VUNSET)) != VUNSET)
		bits = ~(VTEXT_FIXED | VSTACK | VNO_SAVE | VUNSET);
	else if ((vp->flags & VSTR_FIXED))
		bits = VSTR_FIXED;
	else
	{
		*vpp = vp->next;
		xfree(vp);
		if ((flags & (VTEXT_FIXED | VSTACK | VNO_SAVE)) == VNO_SAVE)
			xfree(s);
		return (vp);
	}
	flags |= vp->flags & bits;
	return (NULL);
}

variable_not_found(char *s, int flags, t_var *vp, t_var **vpp)
{
	if ((flags & (VEXPORT | VREAD_ONLY | VSTR_FIXED | VUNSET)) == VUNSET)
	{
		if ((flags & (VTEXT_FIXED | VSTACK | VNO_SAVE)) == VNO_SAVE)
			xfree(s);
		return (vp);
	}
	vp = xmalloc(sizeof(*vp));
	vp->next = *vpp;
	vp->func = NULL;
	*vpp = vp;
}
