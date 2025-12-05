/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_ensure_space.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:59:16 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/05 01:28:20 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_queue.h"

void deque_ensure_space(t_deque *ret)
{
    t_deque new;

    // If cap is zero but len is also zero, allocate an initial buffer.
    if (ret->cap == 0)
    {
        size_t initial_cap = 8;
        if (deque_init(&new, initial_cap, ret->elem_size ? ret->elem_size : 1, ret->ctx))
            ft_assert(!"deque_init failed in deque_ensure_space");
        new.cmp = ret->cmp;
        // swap 'new' into 'ret'
        *ret = new;
        return;
    }
    if (ret->len < ret->cap)
        return;
    deque_init(&new, ret->cap * 2 + 1, ret->elem_size, ret->ctx);
    new.cmp = ret->cmp;
    while (ret->len)
        deque_push_end(&new, deque_pop_start(ret));
    free(ret->buf);
    ft_memcpy(ret, &new, sizeof(new));
    return;
}