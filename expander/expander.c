/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 18:41:10 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/12 19:08:20 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"
#include "lexer.h"
#include "internals/ft_vector.h"
#include "dyn_string.h"
#include <stdlib.h>
#include <string.h>

char *expand_word_single(t_vec *env, t_ast_node *curr)
{
	t_vec args;
	char *ret;

	if (vec_init(&args, &(t_vec_config){
							.initial_capacity = 64,
							.elem_size = sizeof(char *),
							.type_mask = VEC_TYPE_PTR,
							.copy_fn = NULL,
							.free_fn = NULL}) != 0)
		return (NULL);
	expand_word(curr, &args, false, env);
	if (args.len == 1)
	{
		char **slot = (char **)vec_idx(&args, 0);

		if (slot)
			ret = *slot;
		if (slot)
			*slot = NULL;
	}
	vec_destroy(&args);
	return (ret);
}

t_tok get_old_token(t_ast_node word)
{
	t_tok ret;

	ft_assert(word.node_type == AST_WORD);
	ft_assert(word.children.len > 0);
	{
		t_ast_node *child = (t_ast_node *)vec_idx(&word.children, 0);

		ft_assert(child && child->token != NULL);
		ret = child->token->full_word;
	}
	return (ret);
}

void expand_node_glob(t_ast_node *node, t_vec *args, bool keep_as_one)
{
	t_vec glob_words;
	size_t j;

	glob_words = expand_word_glob(node);
	if (get_g_sig()->should_unwind)
	{
		vec_destroy(&glob_words);
		return;
	}
	if (keep_as_one)
	{
		t_dyn_str tmp;

		dyn_str_init(&tmp);
		j = 0;
		while (j < glob_words.len)
		{
			char **slot = (char **)vec_idx(&glob_words, j);
			char *s = slot ? *slot : NULL;

			if (s)
			{
				dyn_str_pushnstr(&tmp, s, ft_strlen(s));
				free(s);
				*slot = NULL;
			}
			if (++j < glob_words.len)
				dyn_str_push(&tmp, ' ');
		}
		if (tmp.buff)
			vec_push(args, &tmp.buff);
		vec_destroy(&glob_words);
		free_ast(node);
		return;
	}
	j = 0;
	while (j < glob_words.len)
	{
		char **slot = (char **)vec_idx(&glob_words, j);
		char *s = slot ? *slot : NULL;

		if (s)
		{
			vec_push(args, &s);
			*slot = NULL;
		}
		j++;
	}
	vec_destroy(&glob_words);
	free_ast(node);
}

void expand_word(t_ast_node *node, t_vec *args, bool keep_as_one, t_vec *env)
{
	t_vec words;
	size_t i;

	expand_tilde_word();
	expand_env_vars(env, node);
	if (!keep_as_one)
		words = split_words(node, env);
	else
	{
		if (vec_init(&words, &(t_vec_config){
								.initial_capacity = 1,
								.elem_size = sizeof(t_ast_node),
								.type_mask = VEC_TYPE_PLAIN,
								.copy_fn = NULL,
								.free_fn = NULL}) != 0)
			return;
		vec_push(&words, node);
		*node = (t_ast_node){0};
	}
	i = 0;
	while (i < words.len)
	{
		t_ast_node *w = (t_ast_node *)vec_idx(&words, i);

		expand_node_glob(w, args, keep_as_one);
		if (get_g_sig()->should_unwind)
		{
			while (i < words.len)
			{
				t_ast_node *rem = (t_ast_node *)vec_idx(&words, i);

				free_ast(rem);
				i++;
			}
			break;
		}
		i++;
	}
	vec_destroy(&words);
	*node = (t_ast_node){0};
}
