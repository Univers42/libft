/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_slice.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 13:32:22 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/07 13:33:45 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"

bool    str_slice_eq_str(char *s, size_t len, char *s2)
{
    size_t  len_s2;

    len_s2 = ft_strlen(s2);
    if (len != len_s2)
        return (false);
    if (ft_strncmp(s, s2, len) == 0)
        return (true);
    return (false);
}
