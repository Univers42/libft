/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias_set.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 01:33:54 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/26 01:33:55 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libalias.h"

static void	update_existing_alias(t_alias *ap, const char *val)
{
	if (!(ap->flag & ALIAS_INUSE))
		free(ap->val);
	ap->val = ft_strdup(val);
	ap->flag &= ~ALIAS_DEAD;
}

static void	create_new_alias(t_alias **app, const char *name, const char *val)
{
	t_alias	*ap;

	ap = malloc(sizeof(t_alias));
	if (!ap)
		return ;
	ap->name = ft_strdup(name);
	ap->val = ft_strdup(val);
	ap->flag = 0;
	ap->next = NULL;
	*app = ap;
}

void	alias_set(const char *name, const char *val)
{
	t_alias	**app;
	t_alias	*ap;

	app = alias_find_slot(name);
	ap = *app;
	if (ap)
		update_existing_alias(ap, val);
	else
		create_new_alias(app, name, val);
}
