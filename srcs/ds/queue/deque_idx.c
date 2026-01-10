/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_idx.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 00:07:42 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/04 17:47:57 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_queue.h"

void	*deque_idx(t_deque *ret, int idx)
{
    void	*src;

    ft_assert(ret != NULL);
    ft_assert(idx >= 0 && (size_t)idx < ret->len);
    src = (char *)ret->buf + (((ret->head + (size_t)idx) % ret->cap) * ret->elem_size);
    return (src);
}
