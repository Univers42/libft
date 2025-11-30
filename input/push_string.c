/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_string.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 23:01:10 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/30 03:57:50 by dlesieur         ###   ########.fr       */
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
    t_parse_file    *pf;
    t_block_str *sp;
    size_t len;

    (void)ap;
    in = get_input();
    pf = get_input()->parsefile;
    len = ft_strlen(s);
    sp = (t_block_str *)xmalloc(sizeof(t_block_str));
    sp->prev = in->parsefile->strpush;
    sp->prev_string = in->parsefile->nextc;
    sp->prevnleft = in->parsefile->nleft;
    sp->ap = NULL;
    sp->string = s;
    sp->sp_free = NULL;
    sp->unget = 0;
    pf->strpush = sp;
    pf->nextc = s;
    pf->nleft = (int)len;
    pf->unget = 0;
}
