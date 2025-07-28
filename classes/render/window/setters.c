/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 17:41:09 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/28 17:54:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"

bool    set_size(t_Window window, int width, int height)
{
    if (!window)
        return (false);
    window.width = width;
    window.height = height;
}
