/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pop_file.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 23:00:20 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/05 00:44:55 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"
#include <stdlib.h>
#include <unistd.h>

/* forward declarations for functions defined elsewhere */
void freestrings(t_block_str *sp);
void popstring(void);

/*
** popfile - pop current parsefile and restore previous one.
*/

void pop_file(void)
{
	t_parse_file *pf;
	t_parse_file *parse_file;

	parse_file = get_input()->parsefile;
	pf = get_input()->parsefile;
	intoff();
	parse_file = pf->prev;
	pf->prev = NULL;
	if (pf == &get_input()->basepf)
		inton();
	if (pf->fd >= 0)
		close(pf->fd);
	xfree(pf->buf);
	if (parse_file->sp_free)
		/* call existing freestrings implementation (no underscore) */
		freestrings(parse_file->sp_free);
	while (pf->strpush)
	{
		/* call existing popstring implementation (no underscore) */
		popstring();
		/* ensure any delayed sp_free is cleared using existing function */
		freestrings(parse_file->sp_free);
	}
	xfree(pf);
}
