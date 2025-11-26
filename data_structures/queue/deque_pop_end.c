/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_pop_end.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:59:22 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/26 01:41:44 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_queue.h"

char	deque_char_pop_end(t_deque_char *ret)
{
	char	i;

	ft_assert(ret->len > 0);
	i = ret->buff[ret->end];
	ret->len--;
	if (ret->len)
		ret->end = ft_smod(ret->end - 1, ret->cap);
	return (i);
}

int	deque_int_pop_end(t_deque_int *ret)
{
	int	i;

	ft_assert(ret->len > 0);
	i = ret->buff[ret->end];
	ret->end = ft_smod(ret->end - 1, ret->cap);
	ret->len--;
	return (i);
}
