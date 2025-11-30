/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dyn_str_ends_with_str.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:36:06 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/30 23:38:03 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dyn_string.h"

bool	dyn_str_ends_with_str(t_dyn_str *s, char *s2)
{
	size_t	len_s2;

	len_s2 = ft_strlen(s2);
	if (s->len < len_s2)
		return (false);
	if (ft_strcmp(s->buff + s->len - len_s2, s2) == 0)
		return (true);
	return (false);
}
