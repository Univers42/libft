/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_strings.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 22:59:10 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/23 23:03:27 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"
#include <stdlib.h>

/*
** freestrings - free a linked list of delayed strpush entries.
** Mirrors the original behaviour but keeps it simple: clears alias flags,
** frees allocated entries except the basestrpush node.
*/

void freestrings(t_block_str *sp)
{
    t_input *in;
    t_block_str *psp;

    in = get_input();
    while (sp)
    {
        if (sp->ap)
        {
        }
        psp = sp;
        sp = sp->sp_free;
        if (psp != &(in->parsefile->basestrpush))
            free(psp);
    }
    in->parsefile->sp_free = NULL;
}
