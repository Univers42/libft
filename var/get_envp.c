/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_envp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 17:49:13 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/05 18:23:55 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private_var.h"

char **get_envp(t_vec *env)
{
	char **ret;
	size_t i;
	size_t j;
	t_env *curr;
	t_dyn_str s;

	if (!env)
		return (NULL);
	ret = xmalloc((env->len + 1) * sizeof(char *));
	if (!ret)
		return (NULL);
	i = 0;
	j = 0;
	while (i < env->len)
	{
		curr = (t_env *)vec_idx(env, i);
		if (curr && curr->exported)
		{
			dyn_str_init(&s);
			dyn_str_pushstr(&s, curr->key);
			dyn_str_push(&s, '=');
			dyn_str_pushstr(&s, curr->value);
			ret[j++] = s.buff;
		}
		i++;
	}
	ret[j] = NULL;
	return (ret);
}
