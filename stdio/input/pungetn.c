/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pungetn.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 23:00:48 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/23 23:03:23 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"

/*
** pungetn - increment unget count on current parsefile.
*/

void pungetn(int n)
{
    t_input *in;

    in = get_input();
    in->parsefile->unget += n;
}
