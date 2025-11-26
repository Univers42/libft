/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pop_file.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 23:00:20 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/24 00:16:56 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"
#include <stdlib.h>
#include <unistd.h>

/*
** popfile - pop current parsefile and restore previous one.
*/

void popfile(void)
{
	t_input			*in;
	t_parse_file	*pf;

	intoff();
	in = get_input();
	pf = pf->prev;
	pf->prev = NULL;
	if (pf == in->basepf)
		inton();
	if (pf->fd >= 0)
		close(pf->fd);
	ckfree(pf->buf);
	if (pf->spfree)
		free_strings(pf->sp_free);
	while (pf->strpush)
	{
		pop_string();
		free_strings(pf->sp_free);
	}
	ckfree(pf);
}
