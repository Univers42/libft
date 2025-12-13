/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_simple_cmd.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 20:26:18 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/13 16:47:23 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"
#include "executor.h"

bool	is_export(t_ast_node word)
{
	t_ast_node	c;

	if (word.children.len != 1)
		return (false);
	c = word.children.buff[0];
	if (c.token.tt != TOKEN_WORD)
		return (false);
	if (ft_strncmp(c.token.start, "export", c.token.len))
		return (false);
	return (true);
}

int	expand_simple_cmd_assignment(t_state *state,
		t_expander_simple_cmd *exp, t_executable_cmd *ret)
{
	if (!exp->found_first)
		vec_env_push(&ret->pre_assigns, assignment_to_env(state, exp->curr));
	else
	{
		assignment_word_to_word(exp->curr);
		if (exp->export)
			expand_word(state, exp->curr, &ret->argv, true);
		else
			expand_word(state, exp->curr, &ret->argv, false);
		if (get_g_sig()->should_unwind)
			return (1);
	}
	return (0);
}

int	expand_simple_cmd_redir(char **context,
		t_expander_simple_cmd *exp, t_vec *redirects)
{
	int			redir_idx;

	if (redirect_from_ast_redir(context, exp->curr, &redir_idx))
		return (AMBIGUOUS_REDIRECT);
	vec_push(redirects, redir_idx);
	return (0);
}

int	expand_simple_cmd_word(t_ast_node *node, t_vec *args
		t_expander_simple_cmd *exp, t_executable_cmd *ret)
{
	if (!exp->found_first && is_export(*exp->curr))
		exp->export = true;
	expand_word(state, exp->curr, &ret->argv, false);
	if (get_g_sig()->should_unwind)
		return (1);
	exp->found_first = true;
	return (0);
}

int	expand_simple_command(t_state *state, t_ast_node *node,
		t_executable_cmd *ret, t_vec *redirects)
{
	t_expander_simple_cmd	exp;

	exp = (typeof(exp)){};
	*ret = (t_executable_cmd){};
	vec_str_init(&ret->argv);
	while (exp.i < node->children.len)
	{
		exp.curr = &node->children.buff[exp.i];
		if (exp.curr->node_type == AST_WORD)
			exp.exit_stat = expand_simple_cmd_word(state, &exp, ret);
		else if (exp.curr->node_type == AST_ASSIGNMENT_WORD)
			exp.exit_stat = expand_simple_cmd_assignment(state, &exp, ret);
		else if (exp.curr->node_type == AST_REDIRECT)
			exp.exit_stat = expand_simple_cmd_redir(state, &exp, redirects);
		else
			ft_assert("Unimplemented" == 0);
		if (exp.exit_stat)
			return (exp.exit_stat);
		exp.i++;
	}
	return (0);
}
