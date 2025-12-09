/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dyn_str_double.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:35:36 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/09 01:46:31 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dyn_string.h"

int dyn_str_double(t_dyn_str *v)
{
    char *tmp;
    size_t newcap;

    if (!v)
        return (0);
    newcap = (v->cap == 0) ? 64 : v->cap * 2;
    tmp = malloc(newcap);
    if (!tmp)
        return (0);
    if (v->buff && v->len)
        memcpy(tmp, v->buff, v->len);
    tmp[v->len] = 0;
    free(v->buff);
    v->buff = tmp;
    v->cap = newcap;
    return (1);
}