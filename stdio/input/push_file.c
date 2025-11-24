/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 23:00:53 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/23 23:03:23 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"
#include <stdlib.h>
#include <string.h>

/*
** pushfile - push a new parsefile onto the stack.
*/

void pushfile(void)
{
    t_input *in;
    t_parse_file *pf;

    in = get_input();
    pf = (t_parse_file *)malloc(sizeof(t_parse_file));
    if (!pf)
        return;
    memset(pf, 0, sizeof(*pf));
    pf->prev = in->parsefile;
    pf->linno = 1;
    pf->fd = -1;
    in->parsefile = pf;
}
