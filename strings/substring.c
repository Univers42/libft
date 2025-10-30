/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   substring.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 16:57:01 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/30 16:59:26 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"

char *substring(const char *string, size_t start, size_t end)
{
    size_t  len;
    size_t  len_safe;
    char    *result;

    len = end - start;
    result = (char *)malloc(len + 1)
    if (!result)
        return ((char *)NULL);
    len_safe = ft_strlcpy(result, string + start, len);
    return (result);
}