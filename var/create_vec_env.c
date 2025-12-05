/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_vec_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 17:40:32 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/05 18:29:19 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private_var.h"

t_vec create_vec_env(char **envp)
{
    t_vec ret;
    t_vec_config config;
    t_env entry;

    ft_memset(&ret, 0, sizeof(ret));
    if (!envp)
        return (ret);
    config.elem_size = sizeof(t_env);
    config.initial_capacity = 32;
    config.type_mask = VEC_TYPE_CUSTOM;
    config.copy_fn = NULL;
    config.free_fn = NULL;
    if (vec_init(&ret, &config) < 0)
        return (ret);
    while (*envp)
    {
        entry = str_to_env(*envp);
        vec_push(&ret, &entry);
        envp++;
    }
    return (ret);
}