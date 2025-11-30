/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_input_string.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 23:01:23 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/30 03:49:58 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"
#include <string.h>
#include "ft_string.h"

/*
** setinputstring - set current parsefile to a provided string.
** This function assumes interrupts are disabled by caller when needed.
*/

void set_input_string(char *string)
{
    t_input *in;

    
    in = get_input();
    intoff();
    push_file();
    in->parsefile->nextc = string;
    in->parsefile->nleft = (int)ft_strlen(string);
    in->parsefile->eof = 2;
    inton();
}
