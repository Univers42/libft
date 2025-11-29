/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_var.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 16:09:31 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/29 14:38:22 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private_var.h"
#include "var.h"

static char	**process_var_bucket(struct s_var *vp, char **ep,
									int mask, int on);
static void	print_var_list(char **ep, char **epend, const char *prefix);

/**
 * API public functions
 */
char	**list_vars(int on, int off, char ***end)
{
	t_var_state	*state;
	t_var		**vpp;
	char		**ep;
	int			mask;

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
	if (ep == stack_str_end())
		ep = grow_stack_str();
	if (end)
		*end = ep;
	*ep++ = NULL;
	return (grab_stack_str(ep));
}

int	show_vars(const char *prefix, int on, int off)
{
	char	**ep;
	char	**epend;
	int		count;

	ep = list_vars(on, off, &epend);
	count = epend - ep;
	ft_qsort(ep, count, sizeof(char *), var_vpcmp);
	print_var_list(ep, epend, prefix);
	return (0);
}

/**
 * PRIVATE HELPERS
 */
static char	**process_var_bucket(struct s_var *vp, char **ep,
									int mask, int on)
{
	while (vp)
	{
		if ((vp->flags & mask) == on)
		{
			if (ep == stack_str_end())
				ep = grow_stack_str();
			*ep++ = (char *)vp->text;
		}
		vp = vp->next;
	}
	return (ep);
}

static void	print_var_list(char **ep, char **epend, const char *prefix)
{
	const char	*sep;
	const char	*p;
	const char	*q;

	if (*prefix)
		sep = SPC_STR;
	else
		sep = prefix;
	while (ep < epend)
	{
		p = ft_strchrnul(*ep, '=');
		q = NULLSTR;
		if (*p)
			q = single_quote(++p);
		out1fmt("%s%s%.*s%s\n", prefix, sep, (int)(p - *ep), *ep, q);
		ep++;
	}
}
