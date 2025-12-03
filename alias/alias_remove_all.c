/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias_remove_all.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 01:33:52 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/04 00:20:49 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "alias.h"

static void	remove_bucket_aliases(t_alias **app)
{
	t_alias	*ap;
	t_alias	*next_node;

	ap = *app;
	while (ap)
	{
		next_node = alias_free(ap);
		*app = next_node;
		if (ap == next_node)
		{
			app = &ap->next;
			ap = *app;
		}
		else
			ap = next_node;
	}
}

void	alias_remove_all(void)
{
	t_alias_state	*state;
	int				i;

	state = get_alias_state();
	i = 0;
	while (i < ALIAS_TAB_SIZE)
	{
		remove_bucket_aliases(&state->atab[i]);
		i++;
	}
}
