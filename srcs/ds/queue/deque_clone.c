/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_clone.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:59:06 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/04 17:50:46 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_queue.h"

int deque_clone(t_deque *ret, const t_deque *proto)
{
	size_t i;
	void *item;

	if (deque_init(ret, proto->cap, proto->elem_size, proto->ctx))
		return (-1);
	i = 0;
	while (i < proto->len)
	{
		item = deque_idx((t_deque *)proto, (int)i);
		deque_push_end(ret, item);
		i++;
	}
	return (0);
}
