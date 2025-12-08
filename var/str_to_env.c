/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_to_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 17:23:35 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/07 16:33:19 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private_var.h"

t_env str_to_env(char *str)
{
    t_env   ret;
    char    *key_pos;

    key_pos = ft_strchr(str, '=') + 1;
    ft_assert(key_pos != 0);
    ret.exported = true;
    ret.key = xmalloc(key_pos - str);
    ft_strlcpy(ret.key, str, key_pos - str);
    ret.value = ft_strdup(key_pos);
    return (ret);
}
