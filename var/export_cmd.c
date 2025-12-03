/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 16:10:14 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/03 23:43:32 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private_var.h"
#include "error.h"

/*
 * Minimal stubs for export/local command handlers.
 * These avoid using undefined flags or sh_error in this translation unit.
 * Replace with full implementations when integrating with the var system.
 */

int export_cmd(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	/* TODO: implement exporting variables (mark VEXPORT on vars) */
	return (0);
}

int local_cmd(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	/* TODO: implement local variable scoping and validation */
	return (0);
}

// PRIVATE HELPERS

// static void process_export_arg(char *arg, int flag)
// {
// 	t_var *vp;
// 	const char *p;

// 	p = ft_strchr(arg, '=');
// 	if (p != NULL)
// 	{
// 		p++;
// 		set_var(arg, p, flag);
// 	}
// 	else
// 	{
// 		vp = *var_find(var_hash(arg), arg);
// 		if (vp)
// 			vp->flags |= flag;
// 		else
// 			set_var(arg, NULL, flag);
// 	}
// }

// static int parse_export_options(int argc, char **argv, int *show_p)
// {
// 	int i;

// 	i = 1;
// 	*show_p = 0;
// 	while (i < argc)
// 	{
// 		if (argv[i][0] != '-')
// 			break;
// 		if (ft_strcmp(argv[i], "-p") == 0)
// 			*show_p = 1;
// 		else if (ft_strcmp(argv[i], "--") == 0)
// 		{
// 			i++;
// 			break;
// 		}
// 		i++;
// 	}
// 	return (i);
// }
