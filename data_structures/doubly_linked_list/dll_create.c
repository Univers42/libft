/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dll_create.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 22:15:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/14 22:11:35 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_doubly_list.h"

t_doubly_list   *ft_dll_create(void)
{
    t_doubly_list   *list;

    list = malloc(sizeof(t_doubly_list));
    if (!list)
        return (NULL);
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return (list);
}
