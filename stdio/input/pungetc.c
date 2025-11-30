/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pungetc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 23:00:42 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/23 23:03:24 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"

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
