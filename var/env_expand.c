/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 17:57:15 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/08 01:31:16 by dlesieur         ###   ########.fr       */
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

int env_set(t_vec *env, t_env new_entry)
{
	t_env *old;

	if (!env || !new_entry.key)
		return (-1);
	old = env_get(env, new_entry.key);
	if (old)
	{
		free(old->value);
		free(new_entry.key);
		old->value = new_entry.value;
		old->exported = new_entry.exported;
		return (0);
	}
	return (vec_push(env, &new_entry) ? 0 : -1);
}

void env_extend(t_vec *edst, t_vec *esrc, bool do_export)
{
	t_env curr;
	size_t i;

	if (!edst || !esrc)
		return;
	i = 0;
	while (i < esrc->len)
	{
		if (vec_pop(esrc, &curr))
		{
			curr.exported = do_export;
			env_set(edst, curr);
		}
		i++;
	}
	vec_destroy(esrc);
}

t_env *env_nget(t_vec *env, char *key, int len)
{
	t_env *curr;
	int i;

	if (!env || !key)
		return (NULL);
	i = (int)env->len - 1;
	while (i >= 0)
	{
		curr = (t_env *)vec_idx(env, i);
		if (curr && ft_strncmp(key, curr->key, len) == 0 && curr->key[len] == '\0')
			return (curr);
		i--;
	}
	return (NULL);
}
