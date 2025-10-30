/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_push_start.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:59:32 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/31 00:08:22 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_queue.h"

void	deque_char_push_start(t_deque_char *ret, char item)
{
	deque_char_double_if_needed(ret);
	if (ret->len)
		ret->start = ft_smod(ret->start - 1, ret->cap);
	ret->buff[ret->start] = item;
	ret->len++;
}

void	deque_int_push_start(t_deque_int *ret, int item)
{
	ft_assert(ret->cap > ret->len);
	ret->start = ft_smod(ret->start - 1, ret->cap);
	ret->buff[ret->start] = item;
	ret->len++;
}