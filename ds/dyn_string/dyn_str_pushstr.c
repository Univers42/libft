/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dyn_str_pushstr.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:36:35 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/09 01:47:33 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dyn_string.h"

// ...existing code...
#include <string.h>
#include "ft_string.h"

int dyn_str_pushstr(t_dyn_str *v, const char *s)
{
    if (!s)
        return (0);
    return dyn_str_pushnstr(v, s, strlen(s));
}
