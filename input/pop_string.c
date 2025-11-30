/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pop_string.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 23:00:25 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/30 03:56:33 by dlesieur         ###   ########.fr       */
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
    t_input         *in;
    t_block_str     *sp;
    t_parse_file    *pf;

    in = get_input();
    pf = get_input()->parsefile;
    sp = in->parsefile->strpush;
    if (!sp)
        return;
    if (sp->ap && pf->nextc > sp->string)
    {
        if (sp->string != sp->ap->name)
            xfree(sp->string);
    }
    pf->nextc = sp->prev_string;
    pf->nleft = sp->prevnleft;
    pf->unget = sp->unget;
    pf->strpush = sp->prev;
    inton();
    free(sp);
}
