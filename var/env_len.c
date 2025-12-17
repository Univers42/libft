/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_len.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 21:21:28 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/12 21:38:05 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "var.h"
#include "ft_ctype.h"

int env_len(char *line)
{
    int len;

    len = 0;
    if (is_name_char(line[len]))
        len++;
    else
        return (len);
    while (is_int_name(line[len]))
        len++;
    return (len);
}
