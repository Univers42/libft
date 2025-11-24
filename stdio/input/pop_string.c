/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pop_string.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 23:00:25 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/23 23:03:24 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"
#include <stdlib.h>
#include <string.h>

/*
** popstring - restore previous string pushed via pushstring().
*/

void popstring(void)
{
    t_input *in;
    t_block_str *sp;

    in = get_input();
    sp = in->parsefile->strpush;
    if (!sp)
        return;
    if (sp->ap && in->parsefile->nextc > sp->string)
    {
        if (in->parsefile->nextc[-1] == ' ' || in->parsefile->nextc[-1] == '\t')
        {
            /* no-op: compatibility */
        }
        if (sp->string != sp->ap)
            ; /* leave freeing to caller or spfree semantics */
    }
    in->parsefile->nextc = sp->prev_string;
    in->parsefile->nleft = sp->prevnleft;
    in->parsefile->unget = sp->unget;
    in->parsefile->strpush = sp->prev;
    free(sp);
}
