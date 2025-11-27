/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setvareq.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 16:09:37 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/27 16:38:59 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "var.h"

static struct s_var	*create_new_var(struct s_var **vpp, char *s, int flags)
{
	struct s_var	*vp;

	if (flags & VNO_SET)
		return (NULL);
	if ((flags & (VEXPORT | VREAD_ONLY | VSTR_FIXED | VUNSET)) == VUNSET)
	{
		if ((flags & (VTEXT_FIXED | VSTACK | VNO_SAVE)) == VNO_SAVE)
			ckfree(s);
		return (NULL);
	}
	vp = ckmalloc(sizeof(*vp));
	vp->next = *vpp;
	vp->func = NULL;
	*vpp = vp;
	if (!(flags & (VTEXT_FIXED | VSTACK | VNO_SAVE)))
		s = savestr(s);
	vp->text = s;
	vp->flags = flags;
	return (vp);
}

static int	handle_update_unset(struct s_var **vpp, char *s, int flags)
{
	struct s_var	*vp;
	int				total_flags;

	vp = *vpp;
	total_flags = (flags & (VEXPORT | VREAD_ONLY | VSTR_FIXED
				| VUNSET)) | (vp->flags & VSTR_FIXED);
	if (total_flags == VUNSET)
	{
		*vpp = vp->next;
		ckfree(vp);
		if ((flags & (VTEXT_FIXED | VSTACK | VNO_SAVE)) == VNO_SAVE)
			ckfree(s);
		return (1);
	}
	return (0);
}

static int	check_var_permissions(struct s_var *vp, char *s, int flags)
{
	const char	*n;

	if (vp->flags & VREAD_ONLY)
	{
		if (flags & VNO_SAVE)
			ckfree(s);
		n = vp->text;
		sh_error("%.*s: is read only", (int)(strchrnul(n, '=') - n), n);
		return (1);
	}
	if (flags & VNO_SET)
		return (1);
	return (0);
}

static struct s_var	*update_existing_var(struct s_var **vpp, char *s, int flags)
{
	struct s_var	*vp;

	vp = *vpp;
	if (check_var_permissions(vp, s, flags))
		return (vp);
	if (vp->func && (flags & VNO_FUNC) == 0)
		(*vp->func)(strchrnul(s, '=') + 1);
	if ((vp->flags & (VTEXT_FIXED | VSTACK)) == 0)
		ckfree((void *)vp->text);
	if (handle_update_unset(vpp, s, flags))
		return (NULL);
	flags |= vp->flags & ~(VTEXT_FIXED | VSTACK | VNO_SAVE | VUNSET);
	if (!(flags & (VTEXT_FIXED | VSTACK | VNO_SAVE)))
		s = savestr(s);
	vp->text = s;
	vp->flags = flags;
	return (vp);
}

struct s_var	*setvareq(char *s, int flags)
{
	struct s_var	*vp;
	struct s_var	**vpp;
	extern int		aflag;

	flags |= (VEXPORT & (((unsigned)(1 - aflag)) - 1));
	vpp = var_hash(s);
	vpp = var_find(vpp, s);
	vp = *vpp;
	intoff();
	if (vp)
		vp = update_existing_var(vpp, s, flags);
	else
		vp = create_new_var(vpp, s, flags);
	inton();
	return (vp);
}
