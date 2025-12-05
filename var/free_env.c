/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 17:45:52 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/05 18:23:53 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private_var.h"

void free_env(t_vec *env)
{
	size_t i;
	t_env *curr;

	if (!env || !env->buff)
		return;
	i = 0;
	while (i < env->len)
	{
		curr = (t_env *)vec_idx(env, i);
		if (curr)
		{
			free(curr->key);
			free(curr->value);
		}
		i++;
	}
	vec_destroy(env);
}
