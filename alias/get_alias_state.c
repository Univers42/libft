/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_alias_state.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 01:34:01 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/26 01:34:02 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libalias.h"

t_alias_state	*get_alias_state(void)
{
	static t_alias_state	instance;
	static int				is_init;

	if (!is_init)
	{
		ft_bzero(&instance, sizeof(t_alias_state));
		is_init = 1;
	}
	return (&instance);
}
