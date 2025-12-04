/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_pop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:59:25 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/04 17:43:28 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_queue.h"

void	*deque_pop_end(t_deque *ret)
{
    void *src;

    ft_assert(ret->len > 0);
    src = (char *)ret->buf + (ret->tail * ret->elem_size);
    ret->len--;
    if (ret->len)
        ret->tail = (size_t)ft_smod((ssize_t)ret->tail - 1, (ssize_t)ret->cap);
    return (src);
}

void	*deque_pop_start(t_deque *ret)
{
    void	*src;

    ft_assert(ret->len > 0);
    src = (char *)ret->buf + (ret->head * ret->elem_size);
    ret->len--;
    if (ret->len)
        ret->head = (ret->head + 1) % ret->cap;
    return (src);
}