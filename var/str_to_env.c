/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_to_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 17:23:35 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/05 18:29:23 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private_var.h"

t_env str_to_env(char *str)
{
    t_env ret;
    char *eq_pos;
    size_t key_len;

    ft_assert(str != NULL);
    eq_pos = ft_strchr(str, '=');
    if (!eq_pos)
        eq_pos = str + ft_strlen(str);
    key_len = eq_pos - str;
    ret.exported = true;
    ret.key = xmalloc(key_len + 1);
    ft_memcpy(ret.key, str, key_len);
    ret.key[key_len] = '\0';
    ret.value = ft_strdup(eq_pos + (eq_pos[0] == '=' ? 1 : 0));
    return (ret);
}
