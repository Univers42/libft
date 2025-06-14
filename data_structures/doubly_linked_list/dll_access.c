/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dll_access.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 22:15:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/14 22:11:37 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_doubly_list.h"

void    *ft_dll_front(t_doubly_list *list)
{
    if (!list || ft_dll_is_empty(list))
        return (NULL);
    return (list->head->data);
}

void    *ft_dll_back(t_doubly_list *list)
{
    if (!list || ft_dll_is_empty(list))
        return (NULL);
    return (list->tail->data);
}

static t_dll_node   *get_node_at_index(t_doubly_list *list, size_t index)
{
    t_dll_node  *current;
    size_t      i;

    if (index >= list->size)
        return (NULL);
    
    if (index < list->size / 2)
    {
        current = list->head;
        i = 0;
        while (i < index)
        {
            current = current->next;
            i++;
        }
    }
    else
    {
        current = list->tail;
        i = list->size - 1;
        while (i > index)
        {
            current = current->prev;
            i--;
        }
    }
    return (current);
}

void    *ft_dll_get(t_doubly_list *list, size_t index)
{
    t_dll_node  *node;

    if (!list)
        return (NULL);
    node = get_node_at_index(list, index);
    if (!node)
        return (NULL);
    return (node->data);
}

bool    ft_dll_set(t_doubly_list *list, size_t index, void *data)
{
    t_dll_node  *node;

    if (!list)
        return (false);
    node = get_node_at_index(list, index);
    if (!node)
        return (false);
    node->data = data;
    return (true);
}
