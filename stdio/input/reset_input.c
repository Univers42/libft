/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reset_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 19:24:15 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/05 00:45:08 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"

/* forward declaration for flush_input implemented in flush_input.c */
void flush_input(void);

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
