/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_get.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 17:21:34 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/05 18:23:54 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private_var.h"

t_env *env_get(t_vec *env, char *key)
{
    t_env *curr;
    size_t i;

    if (!env || !key)
        return (NULL);
    i = 0;
    while (i < env->len)
    {
        curr = (t_env *)vec_idx(env, i);
        if (curr && ft_strcmp(key, curr->key) == 0)
            return (curr);
        i++;
    }
    return (NULL);
}

char *env_get_ifs(t_vec *env)
{
    t_env *e;

    if (!env)
        return ("\t\n");
    e = env_get(env, "IFS");
    if (!e)
        return ("\t\n");
    return (e->value);
}
