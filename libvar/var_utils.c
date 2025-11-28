/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 16:06:16 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/28 16:37:37 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private_var.h"
#include "var.h"

int		varcmp(const char *p, const char *q)
{
	int	c;
	int	d;

	while (1)
	{
		c = *p;
		d = *q;
		if (c != d)
			break ;
		if (!c || c == '=')
			break ;
		p++;
		q++;
	}
	if (c == '=')
		c = 0;
	if (d == '=')
		d = 0;
	return (c - d);
}

int		var_vpcmp(const void *a, const void *b)
{
	return (varcmp(*(const char **)a, *(const char **)b));
}

t_var	**var_find(struct s_var **vpp, const char *name)
{
	while (*vpp)
	{
		if (varcmp((*vpp)->text, name) == 0)
			break ;
		vpp = &(*vpp)->next;
	}
	return (vpp);
}

//TODO: Change this function by our personal strto in stdlib/strto.h
intmax_t	ft_atomax(const char *s, int base)
{
	char	*endptr;

	return (strtoimax(s, &endptr, base));
}

void	unwindlocalvars(t_localvar_list *stop)
{
	t_var_state	*state;

	state = get_var_state();
	while (state->localvar_stack != stop)
		poplocalvars();
}
