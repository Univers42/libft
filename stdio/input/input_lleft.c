/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_lleft.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 22:59:59 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/23 23:00:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"

/*
** Provide external (non-inline) versions of the inline helpers from input.h.
** This allows callers that expect external symbols to link correctly.
*/

int input_get_lleft(t_parse_file *pf)
{
    /* match semantics from the inline variant in input.h */
    return (pf->lleft);
}

void input_set_lleft(t_parse_file *pf, int len)
{
    /* match semantics from the inline variant in input.h */
    pf->lleft = len;
}
