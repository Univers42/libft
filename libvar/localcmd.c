/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   localcmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 16:10:01 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/27 16:47:58 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "var.h"

int	localcmd(int argc, char **argv)
{
	t_var_state	*state;
	int			i;

	state = get_var_state();
	if (!state->localvar_stack)
		sh_error("not in a function");
	i = 1;
	while (i < argc)
	{
		mklocal(argv[i], 0);
		i++;
	}
	return (0);
}
