/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stdin_bufferable.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 23:01:25 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/23 23:03:20 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"

/*
** stdin_bufferable - return whether stdin can be buffered:
** uses stdin_istty and stdin_state members.
*/

int stdin_bufferable(void)
{
    t_input *in;

    in = get_input();
    if (in->stdin_istty < 0)
        return (0);
    return (in->stdin_state.canon || in->stdin_state.seekable);
}
