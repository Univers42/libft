/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reset_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 19:24:15 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/23 23:04:47 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"

/*
** reset_input - reset input state using the input singleton (no globals).
*/
void reset_input(void)
{
	t_input *in;

	in = get_input();
	in->stdin_istty = -1;
	in->basepf.eof = 0;
	flush_input();
}