/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:59:19 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/08 01:03:40 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_queue.h"
#include "ft_memory.h"

int deque_init(t_deque *ret, size_t initial_cap, size_t elem_size, void *ctx)
{
    *ret = (t_deque){0};
    if (initial_cap == 0 || elem_size == 0)
        return (1);
    ret->buf = xmalloc(initial_cap * elem_size);
    if (ret->buf == NULL)
        return (1);
    ret->cap = initial_cap;
    ret->elem_size = elem_size;
    ret->cmp = ft_memcmp;
    ret->ctx = ctx;
    return (0);
}

void deque_clear(t_deque *ret)
{
    ret->len = 0;
    ret->head = 0;
    ret->tail = 0;
    ret->cmp = ft_memcmp;
}

/* new: destroy deque and free underlying buffer */
void deque_destroy(t_deque *ret)
{
    if (!ret)
        return;
    if (ret->buf)
        free(ret->buf);
    /* clear structure to avoid dangling pointers */
    ft_memset(ret, 0, sizeof(t_deque));
}