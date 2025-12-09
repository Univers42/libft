/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 17:57:15 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/09 01:40:32 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private_var.h"

char *env_expand_n(char *last_cmd_status_s, char *pid, char *key, int len, t_vec *env)
{
    t_env *curr;

    if (!key)
        return (NULL);
    if (ft_strncmp(key, "?", len) == 0 && len == 1)
        return (last_cmd_status_s);
    else if (ft_strncmp(key, "$", len) == 0 && pid && len == 1)
        return (pid);
    else if (len == 0)
        return ("$");
    curr = env_nget(env, key, len);
    if (!curr || !curr->key)
        return (NULL);
    return (curr->value);
}

char *env_expand(char *last_cmd_status_s, char *pid, char *key, t_vec *env)
{
	return (env_expand_n(last_cmd_status_s, pid,  key, ft_strlen(key), env));
}

int env_set(t_vec *env, t_env nw)
{
	t_env *old;

	ft_assert(nw.key != 0);
	old = env_get(env, nw.key);
	if (old)
	{
		free(old->value);
		free(nw.key);
		old->value = nw.value;
		old->exported = nw.exported;
	}
	else
		return (vec_push(env, &nw));
	return (0);
}


void env_extend(t_vec *dest, t_vec *src, bool exp)
{
	t_env *curr;
	t_env temp;
	size_t i;

	i = 0;
	while (i < src->len)
	{
		curr = (t_env *)vec_idx(src, i);
		temp = *curr;
		temp.exported = exp;
		env_set(dest, temp);
		i++;
	}
	vec_destroy(src);
}


t_env *env_nget(t_vec *env, char *key, int len)
{
	t_env *curr;
	int i;

	i = env->len - 1;
	while (i >= 0)
	{
		curr = vec_idx(env, i);
		if (ft_strncmp(key, curr->key, len) == 0 && curr->key[len] == 0)
			return (curr);
		i--;
	}
	return (0);
}

void free_env(t_vec *env)
{
	size_t i;
	t_env *curr;

	i = 0;
	while (i < env->len)
	{
		curr = (t_env *)vec_idx(env, i);
		free(curr->key);
		free(curr->value);
		i++;
	}
	free(env->buff);
	env->buff = 0;
}
