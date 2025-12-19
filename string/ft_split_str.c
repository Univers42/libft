/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_str.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 17:50:44 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/19 03:43:58 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "ft_string.h"
#include "ft_memory.h"

char **ft_split_str(char *str, char *sep)
{
    char    **out;
    int     i;
    int     start;
    int     occ;
    int     end;

    out = xcalloc((num_blocks(str, sep) + 1), sizeof(char *));
    if (out == 0)
        return (0);
    i = 0;
    occ = 0;
    while (str[i] != 0)
    {
        start = find_block(&end, str + i, sep);
        if (start == -1)
            return (out);
        out[occ] = malloc(end - start + 1);
        if (out[occ] == 0)
            return (free_list((void **)out, occ), NULL);
        ft_strlcpy(out[occ++], str + i + start, end - start + 1);
        i += end;
    }
    return (out);
}
