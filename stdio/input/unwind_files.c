/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unwind_files.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 23:42:05 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/05 00:44:54 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"
#include <stdlib.h>
#include <unistd.h>

/* forward declaration for pop_file implemented in pop_file.c */
void pop_file(void);

/*
** unwindfiles - pop input files until 'stop' using singleton state
** Mirrors dash: popfile until parsefile == stop and basepf.prev cleared.
*/

void unwindfiles(t_parse_file *stop)
{
	t_input *in;

	in = get_input();
	while (in->basepf.prev || in->parsefile != stop)
		pop_file();
}
