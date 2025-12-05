/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 21:44:33 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/05 22:06:26 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ds.h"
#include "ft_stdio.h"
#include <readline/readline.h>
#include <stdio.h>
#include "lexer.h"
#include "ft_stddef.h"

#define ANSI_RED "\033[31m"
#define ANSI_GREEN "\033[32m"
#define ANSI_RESET "\033[0m"
#define PROMPT "❯ "

t_dyn_str prompt_more_input(t_vec *stack)
{
	t_dyn_str ret;
	t_token_type curr;
	size_t i;
	char buffer[512];
	size_t offset;

	dyn_str_init(&ret);
	offset = 0;
	i = 0;
	while (i < stack->len && offset < sizeof(buffer) - 10)
	{
		curr = *(t_token_type *)vec_idx(stack, i++);
		if (curr == TOKEN_LEFT_BRACE)
			offset += ft_snprintf(buffer + offset, sizeof(buffer) - offset, "subsh ");
		else if (curr == TOKEN_PIPE)
			offset += ft_snprintf(buffer + offset, sizeof(buffer) - offset, "pipe ");
		else if (curr == TOKEN_LOGICAL_AND)
			offset += ft_snprintf(buffer + offset, sizeof(buffer) - offset, "cmdand ");
		else if (curr == TOKEN_LOGICAL_OR)
			offset += ft_snprintf(buffer + offset, sizeof(buffer) - offset, "cmdor ");
	}
	if (offset > 0)
		buffer[offset - 1] = '>';
	dyn_str_pushstr(&ret, buffer);
	dyn_str_pushstr(&ret, " ");
	return (ret);
}

t_dyn_str prompt_normal(char *last_cmd_st, size_t st)
{
	t_dyn_str ret;
	char buffer[256];

	dyn_str_init(&ret);
	if (st == 0)
		ft_snprintf(buffer, sizeof(buffer), "%s%s%s",
					ANSI_GREEN, PROMPT, ANSI_RESET);
	else
		ft_snprintf(buffer, sizeof(buffer), "%s%s %s%s",
					ANSI_RED, last_cmd_st, PROMPT, ANSI_RESET);
	dyn_str_pushstr(&ret, buffer);
	return (ret);
}