/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dyn_str_pushnstr.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:36:33 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/09 01:47:26 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dyn_string.h"

// ...existing code...
#include <string.h>
#include "ft_string.h"

int dyn_str_pushnstr(const char *s, size_t n)
{
    size_t need;

    if (!v || !s)
        return (0);
    need = v->len + n + 1;
    while (!v->buff || need > v->cap)
    {
        if (!dyn_str_double(v))
            return (0);
    }
    memcpy(v->buff + v->len, s, n);
    v->len += n;
    v->buff[v->len] = 0;
    return (1);
}
