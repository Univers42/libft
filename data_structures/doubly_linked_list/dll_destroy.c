/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dll_destroy.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 22:15:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/14 22:11:35 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_doubly_list.h"

void    ft_dll_destroy(t_doubly_list *list)
{
    if (!list)
        return;
    ft_dll_clear(list);
    free(list);
}
