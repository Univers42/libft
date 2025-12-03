/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias_lookup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 01:33:46 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/04 00:20:42 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "alias.h"

t_alias	*alias_lookup(const char *name, int check)
{
	t_alias	**app;
	t_alias	*ap;

	app = alias_find_slot(name);
	ap = *app;
	if (check && ap && (ap->flag & ALIAS_INUSE))
		return (NULL);
	return (ap);
}
