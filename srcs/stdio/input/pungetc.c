/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pungetc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 23:00:42 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/05 00:41:05 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"

/* forward declaration for pungetn implemented in pungetn.c */
void pungetn(int n);

/*
** pungetc - undo a read (single char), respects eof bit.
*/

void pungetc(void)
{
    t_input *in;

    in = get_input();
    pungetn(1 - (in->parsefile->eof & 1));
    in->parsefile->eof &= ~1;
}
