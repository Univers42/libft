/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_double_if_needed.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:59:16 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/31 00:12:23 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_queue.h"

void	deque_char_double_if_needed(t_deque_char *ret)
{
	t_deque_char	new;

	if (ret->len < ret->cap)
		return ;
	deque_char_init(&new, ret->cap * 2 + 1);
	while (ret->len)
		deque_char_push_end(&new, deque_char_pop_end(ret));
	free(ret->buff);
	ft_memcpy(ret, &new, sizeof(new));
	return ;
}
