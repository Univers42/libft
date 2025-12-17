/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dyn_str_ends_with_str.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:36:06 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/09 01:46:39 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dyn_string.h"

int dyn_str_ends_with_str(t_dyn_str *v, const char *s)
{
    size_t sl;
    if (!v || !v->buff || !s)
        return 0;
    sl = strlen(s);
    if (v->len < sl)
        return 0;
    return (memcmp(v->buff + v->len - sl, s, sl) == 0);
}
