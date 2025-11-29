/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   accessors.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 15:37:04 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/29 15:41:40 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "var.h"
#include "private_var.h"

char	*get_optlist(void)
{
	static char	optlist[NOPTS] = {0};

	return (optlist);
}

t_localvar_list	*get_localvar_stack(void)
{
	static t_localvar_list	localvar_stack = {0};

	return (&localvar_stack);
}
