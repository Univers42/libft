/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_tilde.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 19:04:13 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/12 20:30:32 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "expander.h"
#include "ft_string.h"
#include "var.h"
#include "dyn_string.h"

static inline bool token_starts_with(const t_token *t, const char *str)
{
	if (!t || !t->start || !str)
		return (false);
	if (t->len < (int)ft_strlen(str))
		return (false);
	return (ft_strncmp(t->start, str, ft_strlen(str)) == 0);
}

void expand_tilde_token(t_vec *env, t_token *t)
{
	int template_len;
	char *env_val;
	t_dyn_str s;

	if (!t || !t->start)
		return;
	template_len = 1;
	env_val = NULL;
	if (token_starts_with(t, "~+"))
	{
		env_val = env_expand_n(NULL, NULL, "PWD", 3, env);
		template_len = 2;
	}
	else if (token_starts_with(t, "~-"))
	{
		env_val = env_expand_n(NULL, NULL, "OLDPWD", 6, env);
		template_len = 2;
	}
	else
	{
		env_val = env_expand_n(NULL, NULL, "HOME", 4, env);
		template_len = 1;
	}
	if (!env_val)
		return;
	dyn_str_init(&s);
	dyn_str_pushstr(&s, env_val);
	if ((size_t)template_len < (size_t)t->len)
		dyn_str_pushnstr(&s, t->start + template_len, t->len - template_len);
	/* free previous start if it was owned */
	if (t->allocated && t->start)
		free((void *)t->start);
	t->start = s.buff;
	t->len = (int)s.len;
	t->allocated = true;
}

void expand_tilde_word(t_ast_node *curr, t_vec *env)
{
	t_ast_node *first_child;
	t_token *first;
	bool should_expand = false;

	if (!curr || curr->children.len == 0)
		return;
	first_child = (t_ast_node *)vec_idx(&curr->children, 0);
	if (!first_child || first_child->node_type != AST_TOKEN || !first_child->token)
		return;
	first = first_child->token;
	if (first->type != TOKEN_WORD)
		return;

	/* decide expansion cases:
	   "~" or "~/" or "~+" or "~+/" or "~-" or "~-/" */
	if (token_starts_with(first, "~"))
	{
		if (first->len == 1)
			should_expand = true;
		else if (first->start[1] == '/')
			should_expand = true;
		else if (first->start[1] == '+' && (first->len == 2 || first->start[2] == '/'))
			should_expand = true;
		else if (first->start[1] == '-' && (first->len == 2 || first->start[2] == '/'))
			should_expand = true;
	}
	if (should_expand)
		expand_tilde_token(env, first);
}