/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue_front.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 22:10:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/14 22:09:36 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_queue.h"

void    *ft_queue_front(t_queue *queue)
{
    if (!queue || ft_queue_is_empty(queue))
        return (NULL);
    return (queue->front->data);
}
