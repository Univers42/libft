/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 16:10:14 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/29 15:55:25 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "private_var.h"
# include "var.h"

//Forward declartions
static void	process_export_arg(char *arg, int flag);
static int	parse_export_options(int argc, char **argv, int *show_p);

//Public API

int	export_cmd(int argc, char **argv)
{
	int	flag;
	int	show_p;
	int	i;

	if (argv[0][0] == 'r')
		flag = VREAD_ONLY;
	else
		flag = VEXPORT;
	i = parse_export_options(argc, argv, &show_p);
	if (show_p || i == argc)
		show_vars(argv[0], flag, 0);
	else
	{
		while (i < argc)
		{
			process_export_arg(argv[i], flag);
			i++;
		}
	}
	return (0);
}

int	local_cmd(int argc, char **argv)
{
	t_var_state	*state;
	int			i;

	state = get_var_state();
	if (!state->localvar_stack)
		sh_error("not in a function");	//TODO: replace by util instead if possible
	i = 1;
	while (i < argc)
	{
		mklocal(argv[i], 0);	//TODO: understand purpose and change it
		i++;
	}
	return (0);
}

//PRIVATE HELPERS

static void	process_export_arg(char *arg, int flag)
{
	t_var		*vp;
	const char	*p;

	p = ft_strchr(arg, '=');
	if (p != NULL)
	{
		p++;
		set_var(arg, p, flag);
	}
	else
	{
		vp = *var_find(var_hash(arg), arg);
		if (vp)
			vp->flags |= flag;
		else
			set_var(arg, NULL, flag);
	}
}

static int	parse_export_options(int argc, char **argv, int *show_p)
{
	int	i;

	i = 1;
	*show_p = 0;
	while (i < argc)
	{
		if (argv[i][0] != '-')
			break ;
		if (ft_strcmp(argv[i], "-p") == 0)
			*show_p = 1;
		else if (ft_strcmp(argv[i], "--") == 0)
		{
			i++;
			break ;
		}
		i++;
	}
	return (i);
}
