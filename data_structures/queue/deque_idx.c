/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_idx.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 00:07:42 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/31 00:18:29 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_queue.h"

char	deque_char_idx(t_deque_char *ret, size_t idx)
{
	char	i;

	ft_assert(idx < ret->len);
	i = ret->buff[(ret->start + idx) % ret->cap];
	return (i);
}

size_t  deque_int_idx(t_deque_int *ret, size_t idx)
{
	size_t  i;

	ft_assert(idx < ret->len);
	i = ret->buff[(ret->start + idx) % ret->cap];
	return (i);
}