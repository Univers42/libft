/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pop_string.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 23:00:25 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/05 00:45:08 by dlesieur         ###   ########.fr       */
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
    t_parse_file *pf;

    in = get_input();
    pf = get_input()->parsefile;
    sp = in->parsefile->strpush;
    if (!sp)
        return;
    /* If the pushed string moved forward, free it only when we know it's owned.
       The alias structure is opaque here (incomplete type), so avoid accessing
       sp->ap->name. If sp->ap is NULL we own the string and can free it. */
    if (pf->nextc > sp->string)
    {
        if (!sp->ap)
            xfree(sp->string);
        /* else: alias-owned string — do not free here */
    }
    pf->nextc = sp->prev_string;
    pf->nleft = sp->prevnleft;
    pf->unget = sp->unget;
    pf->strpush = sp->prev;
    inton();
    free(sp);
}
