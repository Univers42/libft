/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_vec_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 17:40:32 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/07 16:28:47 by dlesieur         ###   ########.fr       */
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

/**
 * by passing hte t_vec_config to vec_init(), we're explicitly
 * tellign the vector:
 * 1. Element size: `config.elem_size = sizeof(t_env) - Store elements that are `sizeof(t_env)`
 * bytes
 * 2. Type informatino: `config.type_mask = VEC_TYPE_CUSTOM` - This is a custom
 * struct, not a pointer
 * 3. Memory amangeement: config.copy_fn = NULL, and the other NULL too
 * - use default memcpy for copying/freeing
 * 
 * So, what happens is that our vector env when we call the funciton to
 * push into the struct vec_push(&env, str_to_env(*envp)) the vector knows
 * - Each element is sizeof(t_env) btes
 * it copies that many butes into the buffer
 * - it manage the t_env struct instances directly
 * 
 * The vector `ret` now has a properly types buffer that stores t_env
 * strutures, not pointers to them. 
 */
t_vec   env_to_vec_env(t_dyn_str *cwd, char **envp)
{
    t_vec           env;
    t_vec_config    config;
    t_env           lentry;

    config = (t_vec_config){.elem_size = sizeof(t_env), .initial_capacity = 32, .type_mask = VEC_TYPE_CUSTOM};
    if (vec_init(&env, &config) < 0)
        return (env);
    while (*envp)
    {
        lentry = str_to_env(*envp);
        vec_push(&env, &lentry);
        envp++;
    }
    if (cwd->len)
        env_set(&env, (t_env){.key = ft_strdup("PWD"), .value = ft_strdup(cwd->buff), .exported = true});
    if (cwd->len)
        env_set(&env, (t_env){.key = ft_strdup("IFS"), .value = ft_strdup(" \t\n"), .exported = false});
    return (env);
}