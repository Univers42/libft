/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dyn_str_idx.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:36:22 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/09 01:46:45 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dyn_string.h"

int	dyn_str_idx(t_dyn_str *v, size_t idx)
{
	ft_assert(idx < v->len);
	return (v->buff[idx]);
}
