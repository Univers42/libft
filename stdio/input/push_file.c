/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 23:00:53 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/20 22:20:44 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"
#include <stdlib.h>
#include <string.h>

/*
** pushfile - push a new parsefile onto the stack.
*/

void push_file(void)
{
    t_input *in;
    t_parse_file *pf;

    in = get_input();
    pf = (t_parse_file *)malloc(sizeof(t_parse_file));
    ft_memset(pf, 0, sizeof(*pf));
    pf->prev = in->parsefile;
    pf->linno = 1;
    pf->fd = -1;
    in->parsefile = pf;
}
