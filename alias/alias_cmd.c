/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 01:33:39 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/04 00:20:32 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "alias.h"

static int	print_all_aliases(void)
{
	t_alias_state	*state;
	t_alias			*ap;
	int				i;

	state = get_alias_state();
	i = 0;
	while (i < ALIAS_TAB_SIZE)
	{
		ap = state->atab[i];
		while (ap)
		{
			alias_print(ap);
			ap = ap->next;
		}
		i++;
	}
	return (0);
}

static int	process_arg(char *arg)
{
	char	*val;
	t_alias	*ap;

	val = ft_strchr(arg, '=');
	if (val == NULL)
	{
		ap = alias_lookup(arg, 0);
		if (ap == NULL)
		{
			ft_putstr_fd("alias: ", 2);
			ft_putstr_fd(arg, 2);
			ft_putstr_fd(": not found\n", 2);
			return (1);
		}
		alias_print(ap);
	}
	else
	{
		*val = '\0';
		alias_set(arg, val + 1);
	}
	return (0);
}

int	alias_cmd(int argc, char **argv)
{
	int	i;
	int	ret;

	if (argc == 1)
		return (print_all_aliases());
	ret = 0;
	i = 1;
	while (i < argc)
	{
		if (process_arg(argv[i]))
			ret = 1;
		i++;
	}
	return (ret);
}
