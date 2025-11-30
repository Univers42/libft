/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_state_atty.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 16:06:57 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/28 16:37:15 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private_var.h"
#include "var.h"

#ifdef ATTY
int add_atty(t_var_state *state, int i)
{
	state->varinit[i++] = make_atty();
	return i;
}
#else
int add_atty(t_var_state *state, int i)
{
	return i;
}
#endif
