/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pop_all_files.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 23:00:16 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/05 00:45:08 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"

/* forward declaration for unwindfiles implemented in unwind_files.c */
void unwindfiles(t_parse_file *stop);

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
