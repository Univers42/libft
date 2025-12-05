/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 17:57:15 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/05 18:23:56 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private_var.h"

char *env_expand(t_hellish *state, char *key)
{
	return (env_expand_n(state, key, ft_strlen(key)));
}

int env_set(t_vec *env, t_env new)
{
	t_env *old;

	if (!env || !new.key)
		return (-1);
	old = env_get(env, new.key);
	if (old)
	{
		free(old->value);
		free(new.key);
		old->value = new.value;
		old->exported = new.exported;
		return (0);
	}
	return (vec_push(env, &new) ? 0 : -1);
}

void env_extend(t_vec *edst, t_vec *esrc, bool export)
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
			curr.exported = export;
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
