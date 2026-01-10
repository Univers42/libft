/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_push.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:59:28 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/04 17:37:54 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_queue.h"

void deque_push_end(t_deque *ret, void *item)
{
    void	*dst;

    deque_ensure_space(ret);
    if (ret->len)
        ret->tail = (ret->tail + 1) % ret->cap;
    dst = (char *)ret->buf + ret->tail * ret->elem_size;
    ft_memcpy(dst, item, ret->elem_size);
    ret->len++;
}

void deque_push_start(t_deque *ret, void *item)
{
    void	*dst;

    deque_ensure_space(ret);
    if (ret->len)
        ret->head = ft_smod(ret->head - 1, ret->cap);
    dst = (char *)ret->buf + ret->head * ret->elem_size;
    ft_memcpy(dst, item, ret->elem_size);
    ret->len++;
}
