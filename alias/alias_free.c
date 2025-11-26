/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 01:33:44 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/26 01:33:45 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libalias.h"

t_alias	*alias_free(t_alias *ap)
{
	t_alias	*next;

	if (ap->flag & ALIAS_INUSE)
	{
		ap->flag |= ALIAS_DEAD;
		return (ap);
	}
	next = ap->next;
	free(ap->name);
	free(ap->val);
	free(ap);
	return (next);
}
