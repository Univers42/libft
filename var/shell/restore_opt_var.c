/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   restore_opt_var.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 17:26:30 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/29 17:26:39 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../private_var.h"

static void	restore_opt_var(t_localvar *lvp, t_var_state *state)
{
	ft_memcpy(state->optlist, lvp->text, NOPTS);
	xfree((void *)lvp->text);
	optschanged();		// !function to create
}
