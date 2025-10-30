/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:59:19 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/31 00:13:37 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_queue.h"

int	deque_char_init(t_deque_char *ret, int size)
{
	*ret = (t_deque_char){0};
	ret->buff = malloc(size * sizeof(char));
	if (ret->buff == 0)
		return (0);
	ret->cap = size;
	return (1);
}

int	deque_int_init(t_deque_int *ret, int size)
{
	*ret = (t_deque_int){0};
	ret->buff = malloc(size * sizeof(int));
	if (ret->buff == 0)
		return (0);
	ret->cap = size;
	ret->end = ret->cap - 1;
	return (1);
}