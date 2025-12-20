/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_var.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 16:09:31 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/20 22:20:44 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private_var.h"
#include "var.h"

static void print_var_list(char **ep, char **epend, const char *prefix);

/* Replace stack-string helpers with simple counters/fillers */
static size_t count_var_bucket(struct s_var *vp, int mask, int on)
{
	size_t n;

	n = 0;
	while (vp)
	{
		if ((vp->flags & mask) == on)
			n++;
		vp = vp->next;
	}
	return (n);
}

static char **fill_var_bucket(struct s_var *vp, char **ep, int mask, int on)
{
	while (vp)
	{
		if ((vp->flags & mask) == on)
			*ep++ = (char *)vp->text;
		vp = vp->next;
	}
	return (ep);
}

/**
 * API public functions
 */
char **list_vars(int on, int off, char ***end)
{
	t_var_state *state;
	t_var **vpp;
	size_t total;
	char **arr;
	char **it;
	int mask;

	state = get_var_state();
	mask = on | off;

	/* First pass: count */
	total = 0;
	vpp = state->vartab;
	while (vpp < state->vartab + VTABSIZE)
	{
		total += count_var_bucket(*vpp, mask, on);
		vpp++;
	}

	/* Allocate result (+1 for NULL sentinel) */
	arr = malloc((total + 1) * sizeof(char *));
	if (!arr)
		return (NULL);

	/* Second pass: fill */
	it = arr;
	vpp = state->vartab;
	while (vpp < state->vartab + VTABSIZE)
	{
		it = fill_var_bucket(*vpp, it, mask, on);
		vpp++;
	}
	*it = NULL;
	if (end)
		*end = arr + total;
	return (arr);
}

int show_vars(const char *prefix, int on, int off)
{
	char **ep;
	char **epend;
	int count;

	ep = list_vars(on, off, &epend);
	if (!ep)
		return (0);
	count = (int)(epend - ep);
	ft_qsort(ep, count, sizeof(char *), var_vpcmp);
	print_var_list(ep, epend, prefix);
	/* Avoid leak: list_vars returns heap memory */
	xfree(ep);
	return (0);
}

static void print_var_list(char **ep, char **epend, const char *prefix)
{
	const char *sep;
	const char *p;
	const char *q;

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
