/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_clone.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:59:06 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/31 00:16:52 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_queue.h"

int	deque_char_clone(t_deque_char *ret, const t_deque_char proto)
{
	size_t	i;

	if (!deque_char_init(ret, proto.cap))
		return (0);
	i = 0;
	while (i < proto.len)
	{
		deque_char_push_end(ret, proto.buff[(proto.start + i) % proto.cap]);
		i++;
	}
	return (1);
}

int	deque_int_clone(t_deque_int *ret, const t_deque_int proto)
{
	size_t	i;

	if (!deque_int_init(ret, proto.cap))
		return (0);
	i = 0;
	while (i < proto.len)
	{
		deque_int_push_end(ret, proto.buff[(proto.start + i) % proto.cap]);
		i++;
	}
	return (1);
}
