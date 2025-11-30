/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pop_file.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 23:00:20 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/30 03:48:02 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"
#include <stdlib.h>
#include <unistd.h>

/*
** popfile - pop current parsefile and restore previous one.
*/

void	pop_file(void)
{
	t_parse_file	*pf;
	t_parse_file	*parse_file;

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
		free_strings(parse_file->sp_free);
	while (pf->strpush)
	{
		pop_string();
		free_strings(parse_file->sp_free);
	}
	xfree(pf);
}
