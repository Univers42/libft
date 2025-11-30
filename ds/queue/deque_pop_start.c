/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_pop_start.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:59:25 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/26 01:42:23 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_queue.h"

char	deque_char_pop_start(t_deque_char *ret)
{
	char	i;

	ft_assert(ret->len > 0);
	i = ret->buff[ret->start];
	ret->len--;
	if (ret->len)
		ret->start = (ret->start + 1) % ret->cap;
	return (i);
}

int	deque_int_pop_start(t_deque_int *ret)
{
	int	i;

	ft_assert(ret->len > 0);
	i = ret->buff[ret->start];
	ret->start = (ret->start + 1) % ret->cap;
	ret->len--;
	return (i);
}
