/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_input_string.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 23:01:23 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/23 23:03:21 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"
#include <string.h>

/*
** setinputstring - set current parsefile to a provided string.
** This function assumes interrupts are disabled by caller when needed.
*/

void setinputstring(char *string)
{
    t_input *in;

    in = get_input();
    in->parsefile->nextc = string;
    in->parsefile->nleft = (int)strlen(string);
    in->parsefile->eof = 2;
}
