/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 21:48:31 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/11 22:03:15 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "var.h"
#include <string.h>

/* Minimal placeholder for unset_func — real implementation should remove
   a shell function definition by name. Kept as a no-op so the library
   builds; replace with proper function-table handling when available. */
void unset_func(const char *name)
{
	(void)name;
	/* no-op placeholder */
}

static int parse_unset_options(int argc, char **argv, int *flag_out);
static void process_unset_args(int argc, char **argv,
							   int start_index, int flag);

// PUBLIC function

int unset_cmd(int argc, char **argv)
{
	int flag;
	int i;

	i = parse_unset_options(argc, argv, &flag);
	process_unset_args(argc, argv, i, flag);
	return (0);
}

void unset_var(char *s)
{
	set_var(s, 0, 0);
}

// PRIVATE
static int parse_unset_options(int argc, char **argv, int *flag_out)
{
	int i;

	i = 1;
	*flag_out = 0;
	while (i < argc && argv[i][0] == '-')
	{
		if (ft_strcmp(argv[i], "-v") == 0)
			*flag_out = 'v';
		else if (ft_strcmp(argv[i], "-f") == 0)
			*flag_out = 'f';
		else if (ft_strcmp(argv[i], "--") == 0)
		{
			i++;
			break;
		}
		else
			break;
		i++;
	}
	return (i);
}

static void process_unset_args(int argc, char **argv, int start_index, int flag)
{
	int i;

	i = start_index;
	while (i < argc)
	{
		if (flag != 'f')
		{
			unset_var(argv[i]);
			if (flag == 'v')
			{
				i++;
				continue;
			}
		}
		if (flag != 'v')
			unset_func(argv[i]);
		i++;
	}
}
