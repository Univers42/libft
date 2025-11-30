/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_stdin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 23:01:03 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/30 03:36:05 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"

/*
** pushstdin - set parsefile to basepf (top-level) so we read from stdin.
*/

void pushstdin(void)
{
    t_input *in;

    intoff();
    in = get_input();
    in->basepf.prev = in->parsefile;
    in->parsefile = &in->basepf;
    inton();
}
