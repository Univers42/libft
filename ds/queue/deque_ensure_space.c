/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_ensure_space.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:59:16 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/04 17:45:40 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_queue.h"

void	deque_ensure_space(t_deque *ret)
{
    t_deque	new;

    if (ret->len < ret->cap)
        return ;
    deque_init(&new, ret->cap * 2 + 1, ret->elem_size, ret->ctx);
    new.cmp = ret->cmp;
    while (ret->len)
        deque_push_end(&new, deque_pop_start(ret));
    free(ret->buf);
    ft_memcpy(ret, &new, sizeof(new));
    return ;
}