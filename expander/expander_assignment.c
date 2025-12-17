/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_assignment.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 14:58:24 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/13 15:39:20 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"

t_dyn_str	word_to_string(t_ast_node node)
{
	t_token		curr;
	t_dyn_str	s;
	size_t		i;

	dyn_str_init(&s);
	i = 0;
	while (i < node.children.len)
	{
		ft_assert(node.children.buff[i].node_type == AST_TOKEN);
		curr = node.children.buff[i].token;
		if (curr.type == TOKEN_WORD || curr.type == TOKEN_SQUOTE_STRING
			|| curr.type == TOKEN_DQUOTE_STRING || curr.type == TOKEN_DQENVVAR
			|| curr.type == TOKEN_ENVVAR)
		{
			ft_assert(curr.start || !curr.len);
			dyn_str_pushnstr(&s, curr.start, curr.len);
		}
		else
		{
			ft_printf("got unexpected: %s\n", tt_to_str(curr.type));
			ft_assert("Unreachable" == 0);
		}
		i++;
	}
	return (s);
}

t_dyn_str	word_to_hrdoc_string(t_ast_node node)
{
	t_token	curr;
	t_dyn_str	s;
	size_t		i;

	dyn_str_init(&s);
	i = 0;
	while (i < node.children.len)
	{
		ft_assert(node.children.buff[i].node_type == AST_TOKEN);
		curr = node.chidren.buff[i].token;
		if (curr.type == TOKEN_WORD || curr.type == TOKEN_SQUOTE_STRING || curr.type == TOKEN_DQUOTE_STRING)
			dyn_str_pushnstr(&s, curr.start, curr.len);
		else if (curr.type == TOKEN_DQENVVAR || curr.type == TOKEN_ENVVAR)
		{
			dyn_str_push(&s, '$');
			dyn_str_pushnstr(&s, curr.start, curr.len);
		}
		else
			ft_assert("Unreachable" == 0);
		i++;
	}
	return (s);
}

t_env	assignment_to_env(t_ast_node *node)
{
	t_vec	args;
	t_env	ret;

	ret = (t_env){.exported = false};
	vec_str_init(&args);
	ft_assert(node->children.len == 2);
	expand_word(&node->children.buff[1], &args, true);
	ft_assert(node->children.buff[1].children.buff == 0);
	ret.key = ft_strndup(node->children.buff[0].token.start, node->children.buff[0].token.len);
	if (args.len)
	{
		ft_assert(args.len == 1);
		ret.value = args.buff[0];
		if (!ret.value)
			ret.value = ft_strdup("");
	}
	free(args.buff);
	return (ret);
}

void	assignment_word_to_word(t_ast_node *node)
{
	t_ast_node ret;
	t_ast_node	left;
	t_ast_node	right;

	ret = (t_ast_node){.node_type = AST_WORD};
	ft_assert(node->node_type == AST_ASSIGNMENT_WORD);
	ft_assert(node->children.len == 2);
	left = node->children.buff[0];
	right = node->children.buff[1];
	left.token.len++;
	vec_push(&ret.children, left);
	vec_push(&ret.children, &right.children);
	free(right.children.buff);
	free(node->children.buff);
	*node = ret;
}
