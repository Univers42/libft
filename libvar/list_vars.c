/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_vars.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 16:10:03 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/27 16:49:53 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "var.h"

static char	**process_var_bucket(struct s_var *vp, char **ep,
									int mask, int on)
{
	while (vp)
	{
		if ((vp->flags & mask) == on)
		{
			if (ep == stackstrend())
				ep = growstackstr();
			*ep++ = (char *)vp->text;
		}
		vp = vp->next;
	}
	return (ep);
}

char	**listvars(int on, int off, char ***end)
{
	struct s_var_state	*state;
	struct s_var		**vpp;
	char				**ep;
	int					mask;

	state = get_var_state();
	ep = NULL;
	STARTSTACKSTR(ep);
	vpp = state->vartab;
	mask = on | off;
	while (vpp < state->vartab + VTABSIZE)
	{
		ep = process_var_bucket(*vpp, ep, mask, on);
		vpp++;
	}
	if (ep == stackstrend())
		ep = growstackstr();
	if (end)
		*end = ep;
	*ep++ = NULL;
	return (grabstackstr(ep));
}
