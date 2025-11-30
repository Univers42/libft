/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pop_all_files.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 23:00:16 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/23 23:03:25 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"

/*
** popallfiles - unwind to top-level (NULL stop)
*/

void popallfiles(void)
{
    t_input *in;

    in = get_input();
    unwindfiles(NULL);
    in->parsefile = &in->basepf;
}
