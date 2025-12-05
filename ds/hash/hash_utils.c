/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 17:15:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/05 17:05:42 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_hash.h"
#include "ft_string.h"

/* FNV-1a hash for strings */
size_t hash_string(const void *key)
{
    const char *str;
    size_t hash;

    if (!key)
        return (0);
    str = (const char *)key;
    hash = 2166136261U;
    while (*str)
    {
        hash ^= (unsigned char)*str;
        hash *= 16777619;
        str++;
    }
    return (hash);
}

int cmp_string(const void *a, const void *b)
{
    if (!a || !b)
        return ((a == b) ? 0 : 1);
    return (ft_strcmp((const char *)a, (const char *)b));
}

/* Hash for pointers */
size_t hash_ptr(const void *key)
{
    return ((size_t)key * 2654435761U);
}

int cmp_ptr(const void *a, const void *b)
{
    if (a == b)
        return (0);
    return ((a < b) ? -1 : 1);
}
