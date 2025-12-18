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

int dyn_str_pushstr(const char *s)
{
    if (!s)
        return (0);
    return dyn_str_pushnstr(s, strlen(s));
}

bool dyn_str_check_at(size_t pos, ctype_fn fn, char *cmp, size_t len)
{
	t_dyn_str	*input;

	input = get_dyn_str(NULL);
    ft_assert(input && input->buff && pos < input->len);
    if (fn)
        return fn((unsigned char)dyn_str_idx(input, pos));
    else
        return (ft_strncmp(input->buff + pos, cmp, len) == 0);
}
