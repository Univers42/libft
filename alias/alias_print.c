/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 01:33:49 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/26 01:33:50 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libalias.h"

void	alias_print(const t_alias *ap)
{
	char	*quoted_val;

	if (!ap)
		return ;
	quoted_val = alias_str_quote(ap->val);
	ft_putstr_fd(ap->name, 1);
	ft_putstr_fd("=", 1);
	ft_putstr_fd(quoted_val, 1);
	ft_putstr_fd("\n", 1);
	free(quoted_val);
}
