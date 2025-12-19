/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_split.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 17:31:56 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/19 04:17:51 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"
#include "ds.h"
#include "ft_glob.h"
#include "lexer.h"
#include "var.h"
#include "ft_string.h"
#include "internals/ft_vector.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// static void expand_token(t_vec *env, t_token *curr_tt)
// {
// 	char *temp;

// 	if (!curr_tt || !curr_tt->start)
// 		return;
// 	temp = env_expand_n(NULL, NULL, curr_tt->start, curr_tt->len, env);
// 	if (temp)
// 	{
// 		curr_tt->start = temp;
// 		curr_tt->len = (int)ft_strlen(temp);
// 		curr_tt->allocated = true;
// 	}
// 	else
// 	{
// 		curr_tt->len = 0;
// 	}
// }

// void expand_env_vars(t_vec *env, t_ast_node *node)
// {
// 	size_t i;

// 	if (!node)
// 		return;
// 	i = 0;
// 	while (i < node->children.len)
// 	{
// 		t_ast_node *child = (t_ast_node *)vec_idx(&node->children, i);
// 		ft_assert(child && child->node_type == AST_TOKEN);
// 		ft_assert(child->token != NULL);
// 		t_token *tok = child->token;
// 		if (tok->type == TOKEN_DQENVVAR || tok->type == TOKEN_ENVVAR)
// 			expand_token(env, tok);
// 		else if (tok->type == TOKEN_WORD || tok->type == TOKEN_SQUOTE_STRING || tok->type == TOKEN_DQUOTE_STRING)
// 		{
// 		}
// 		else
// 			ft_assert("Unreachable" == 0);
// 		i++;
// 	}
// }

// static t_ast_node new_env_node(char *new_start)
// {
// 	t_ast_node n;
// 	t_token *tok;

// 	memset(&n, 0, sizeof(n));
// 	tok = (t_token *)malloc(sizeof(t_token));
// 	ft_assert(tok != NULL);
// 	tok->start = ft_strdup(new_start ? new_start : "");
// 	ft_assert(tok->start != NULL);
// 	tok->len = (int)ft_strlen(tok->start);
// 	tok->type = TOKEN_ENVVAR;
// 	tok->allocated = true;
// 	tok->line = 0;
// 	n.node_type = AST_TOKEN;
// 	n.token = tok;
// 	vec_init(&n.children, &(t_vec_config){.elem_size = sizeof(t_ast_node), .initial_capacity = 0, .type_mask = VEC_TYPE_PLAIN});
// 	return (n);
// }

// static void split_envvar(t_token *curr_t, t_ast_node *curr_node, t_vec *ret, t_vec *env)
// {
// 	char **things;
// 	int i;

// 	if (!curr_t || !curr_t->start)
// 		return;
// 	const char *ifs = " \t\n";
// 	things = ft_split_str(curr_t->start, (char *)ifs);
// 	if (!things)
// 		return;

// 	if (things[0])
// 	{
// 		t_ast_node first = new_env_node(things[0]);
// 		vec_push(&curr_node->children, &first);
// 		i = 1;
// 		while (things[i])
// 		{
// 			vec_push(ret, curr_node);
// 			vec_destroy(&curr_node->children);
// 			*curr_node = (t_ast_node){0};
// 			curr_node->node_type = AST_WORD;
// 			vec_init(&curr_node->children, &(t_vec_config){.elem_size = sizeof(t_ast_node), .initial_capacity = 2, .type_mask = VEC_TYPE_PLAIN});
// 			t_ast_node piece = new_env_node(things[i]);
// 			vec_push(&curr_node->children, &piece);
// 			i++;
// 		}
// 	}
// 	else
// 	{
// 		if (curr_node->children.len)
// 		{
// 			vec_push(ret, curr_node);
// 			vec_destroy(&curr_node->children);
// 			*curr_node = (t_ast_node){0};
// 			curr_node->node_type = AST_WORD;
// 			vec_init(&curr_node->children, &(t_vec_config){.elem_size = sizeof(t_ast_node), .initial_capacity = 2, .type_mask = VEC_TYPE_PLAIN});
// 		}
// 	}
// 	i = 0;
// 	while (things[i])
// 	{
// 		free(things[i]);
// 		i++;
// 	}
// 	free(things);
// }

// t_vec split_words(t_ast_node *node, t_vec *env)
// {
// 	t_vec ret;
// 	t_ast_node curr_node;
// 	int i;

// 	vec_init(&ret, &(t_vec_config){.initial_capacity = 8, .elem_size = sizeof(t_ast_node), .type_mask = VEC_TYPE_PLAIN});
// 	memset(&curr_node, 0, sizeof(curr_node));
// 	curr_node.node_type = AST_WORD;
// 	vec_init(&curr_node.children, &(t_vec_config){.initial_capacity = 4, .elem_size = sizeof(t_ast_node), .type_mask = VEC_TYPE_PLAIN});

// 	i = 0;
// 	while (i < (int)node->children.len)
// 	{
// 		t_ast_node *child = (t_ast_node *)vec_idx(&node->children, i);
// 		ft_assert(child && child->node_type == AST_TOKEN && child->token != NULL);
// 		t_token *ct = child->token;

// 		if (ct->type == TOKEN_WORD || ct->type == TOKEN_SQUOTE_STRING || ct->type == TOKEN_DQUOTE_STRING || ct->type == TOKEN_DQENVVAR)
// 		{
// 			vec_push(&curr_node.children, child);
// 		}
// 		else if (ct->type == TOKEN_ENVVAR)
// 		{
// 			split_envvar(ct, &curr_node, &ret, env);
// 		}
// 		else
// 		{
// 			ft_assert("Unreachable" == 0);
// 		}
// 		i++;
// 	}

// 	if (curr_node.children.len)
// 		vec_push(&ret, &curr_node);

// 	vec_destroy(&node->children);
// 	*node = (t_ast_node){0};
// 	return (ret);
// }
