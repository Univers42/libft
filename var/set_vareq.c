/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_vareq.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 23:44:21 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/11 22:05:33 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "var.h"

static t_var *variable_not_found(char *s, int *pflags, t_var **vpp);
static t_var *variable_exist(char *s, int *pflags, t_var *vp, t_var **vpp);

t_var *set_vareq(char *s, int flags)
{
	t_var *vp;
	t_var **vpp;
	t_var *ret;
	int store_flags;

	/* preserve existing semantics, ensure AFLAG is defined in private_var.h */
	flags |= (VEXPORT & (((unsigned)(1 - get_optlist()[AFLAG])) - 1));
	vpp = find_var(s);
	vp = *vpp;

	if (vp)
	{
		ret = variable_exist(s, &flags, vp, vpp);
		/* read-only: variable_exist already reported error/free as needed */
		if (ret && (ret->flags & VREAD_ONLY))
			return (ret);
		/* Unset requested: nothing more to do; free s if caller said no save */
		if (flags & VUNSET)
		{
			if (flags & VNO_SAVE)
				xfree(s);
			return (NULL);
		}
		/* If variable_exist returned a vp (e.g., VSTR_FIXED case), use it */
		vp = ret ? ret : *vpp;
	}
	else
	{
		/* Create new vp and continue to assign text */
		vp = variable_not_found(s, &flags, vpp);
		if (!vp)
		{
			if (flags & VNO_SAVE)
				xfree(s);
			return (NULL);
		}
	}

	/* Take ownership of s and store flags without internal VNO_SAVE bit */
	store_flags = flags & ~VNO_SAVE;
	vp->text = s;
	vp->flags = store_flags;

	if (vp->func && (flags & VNO_FUNC) == 0)
		(*vp->func)(var_null(s));
	return (vp);
}

static t_var *variable_exist(char *s, int *pflags, t_var *vp, t_var **vpp)
{
	unsigned bits;
	const char *n;
	int flags;
	void *old_text;

	flags = *pflags;
	/* Read-only check */
	if (vp->flags & VREAD_ONLY)
	{
		if ((flags & VNO_SAVE) && s != vp->text)
			xfree(s);
		n = vp->text;
		sh_error("%.*s: is read only", (int)(ft_strchrnul(n, '=') - n));
		return (vp);
	}

	/* Preserve old text pointer (for safety in comparisons) */
	old_text = (void *)vp->text;

	/* Free old text if it's mutable */
	if ((vp->flags & (VTEXT_FIXED | VSTACK)) == 0 && vp->text)
		xfree((void *)vp->text);

	/* If caller asked to unset this variable, remove it from the list.
	   Do NOT free 's' here; set_vareq() will handle it to avoid double-free. */
	if (flags & VUNSET)
	{
		*vpp = vp->next;
		xfree(vp);
		return (NULL);
	}

	/* Preserve a set of bits from the existing variable */
	bits = ~(VTEXT_FIXED | VSTACK | VNO_SAVE | VUNSET);
	flags |= vp->flags & bits;
	*pflags = flags;

	/* If variable had VSTR_FIXED, keep that flag; proceed to assign */
	if (vp->flags & VSTR_FIXED)
	{
		flags |= VSTR_FIXED;
		*pflags = flags;
		return (vp);
	}
	/* Allow caller to proceed with replacing text */
	(void)old_text;
	return (NULL);
}

static t_var *variable_not_found(char *s, int *pflags, t_var **vpp)
{
	int flags;
	t_var *vp;

	flags = *pflags;
	/* If asked to unset a non-existent var, nothing to do (free if required) */
	if (flags & VUNSET)
	{
		if (flags & VNO_SAVE)
			xfree(s);
		return (NULL);
	}
	vp = xmalloc(sizeof(*vp));
	/* Initialize fields to safe defaults and link into list */
	vp->next = *vpp;
	vp->func = NULL;
	vp->text = NULL;
	vp->flags = 0;
	*vpp = vp;
	return (vp);
}
