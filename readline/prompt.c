/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 07:35:12 by anddokhn          #+#    #+#             */
/*   Updated: 2025/12/08 23:38:39 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>
#include "libft.h"
#include "parser.h"
#include "lexer.h"

t_dyn_str prompt_more_input(t_parse *parser)
{
	t_dyn_str ret;
	t_token_type curr;
	size_t i;

	i = 0;
	dyn_str_init(&ret);
	while (i < parser->stack.len)
	{
		curr = (t_token_type)vec_idx(&parser->stack, i++);
		if (curr == TOKEN_LEFT_BRACE)
			dyn_str_pushstr(&ret, "subsh");
		else if (curr == TOKEN_PIPE)
			dyn_str_pushstr(&ret, "pipe");
		else if (curr == TOKEN_LOGICAL_AND)
			dyn_str_pushstr(&ret, "cmdand");
		else if (curr == TOKEN_LOGICAL_OR)
			dyn_str_pushstr(&ret, "cmdor");
		else
			continue;
		dyn_str_pushstr(&ret, " ");
	}
	ret.buff[ret.len - 1] = '>';
	dyn_str_pushstr(&ret, " ");
	return (ret);
}

t_dyn_str prompt_normal(t_status *last_cmd_status_res, char **last_cmd_status_s)
{
	t_dyn_str ret;

	dyn_str_init(&ret);
	if (last_cmd_status_res->status == 0)
		dyn_str_pushstr(&ret, ANSI_GREEN);
	else
	{
		dyn_str_pushstr(&ret, ANSI_RED);
		dyn_str_pushstr(&ret, *last_cmd_status_s);
		dyn_str_pushstr(&ret, " ");
	}
	dyn_str_pushstr(&ret, PROMPT);
	dyn_str_pushstr(&ret, ANSI_RESET);
	dyn_str_pushstr(&ret, RL_SPACER_1);
	dyn_str_pushstr(&ret, RL_SPACER_1);
	return (ret);
}
