/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_push_end.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:59:28 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/26 01:41:57 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_queue.h"

void	deque_char_push_end(t_deque_char *ret, char item)
{
	deque_char_double_if_needed(ret);
	if (ret->len)
		ret->end = (ret->end + 1) % ret->cap;
	ret->buff[ret->end] = item;
	ret->len++;
}

void	deque_int_push_end(t_deque_int *ret, int item)
{
	ft_assert(ret->cap > ret->len);
	ret->end = (ret->end + 1) % ret->cap;
	ret->buff[ret->end] = item;
	ret->len++;
}
