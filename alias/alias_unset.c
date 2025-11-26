/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias_unset.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 01:33:59 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/26 01:34:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libalias.h"

int	alias_unset(const char *name)
{
	t_alias	**app;

	app = alias_find_slot(name);
	if (*app)
	{
		*app = alias_free(*app);
		return (0);
	}
	return (1);
}
