/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_var.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 16:09:31 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/27 16:42:15 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "var.h"

static void	print_var_list(char **ep, char **epend, const char *prefix)
{
	const char	*sep;
	const char	*p;
	const char	*q;

	if (*prefix)
		sep = spcstr;
	else
		sep = prefix;
	while (ep < epend)
	{
		p = strchrnul(*ep, '=');
		q = nullstr;
		if (*p)
			q = single_quote(++p);
		out1fmt("%s%s%.*s%s\n", prefix, sep, (int)(p - *ep), *ep, q);
		ep++;
	}
}

int	showvars(const char *prefix, int on, int off)
{
	char	**ep;
	char	**epend;
	int		count;

	ep = listvars(on, off, &epend);
	count = epend - ep;
	ft_qsort(ep, count, sizeof(char *), var_vpcmp);
	print_var_list(ep, epend, prefix);
	return (0);
}
