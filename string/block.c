/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 18:01:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/19 03:35:31 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"
#include "ft_memory.h"

int find_block(int *end, const char *str, const char *sep)
{
    int i;
    int prev;
    int len;

    i = 0;
    prev = -1;
    len = (int)ft_strlen(str);
    if (len == 0)
        return (-1);
    while (i <= len)
    {
        if (((sep[1] == '\0') ? (str[i] == sep[0]) : (ft_strchr(sep, str[i]) != NULL))
            || !str[i])
        {
            *end = i;
            if ((i - prev) > 1)
                return (prev + 1);
            prev = i;
        }
        i++;
    }
    return (-1);
}

int num_blocks(const char *str, char *sep)
{
    int i;
    int occ;
    int end;
    int start;

    i = 0;
    occ = 0;
    while (str[i] != 0)
    {
        start = find_block(&end, str + i, sep);
        if (start == -1)
            return (occ);
        occ++;
        i += end;
    }
    return (occ);
}
