/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_string.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 23:01:10 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/23 23:03:22 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"
#include <stdlib.h>
#include <string.h>

/*
** pushstring - push a string on current parsefile level.
** Simpler variant of dash pushstring.
*/

void pushstring(char *s, void *ap)
{
    t_input *in;
    t_block_str *sp;
    size_t len;

    (void)ap;
    in = get_input();
    len = strlen(s);
    sp = (t_block_str *)malloc(sizeof(t_block_str));
    sp->prev = in->parsefile->strpush;
    sp->prev_string = in->parsefile->nextc;
    sp->prevnleft = in->parsefile->nleft;
    sp->ap = NULL;
    sp->string = s;
    sp->sp_free = NULL;
    sp->unget = 0;
    in->parsefile->strpush = sp;
    in->parsefile->nextc = s;
    in->parsefile->nleft = (int)len;
    in->parsefile->unget = 0;
}
