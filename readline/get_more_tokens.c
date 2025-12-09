/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_more_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 11:43:56 by anddokhn          #+#    #+#             */
/*   Updated: 2025/12/09 02:36:15 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdbool.h>

// 1 - EOF
//
// 2 - C-c
static int readline_cmd(t_rl *rl, char **prompt, t_dyn_str *input, t_status *last_cmd_status_res, char **last_cmd_status_s, int *input_method, char **context, char **base_context, int *should_exit)
{
	int stat;

	/* prompt memory is managed by caller */
	stat = xreadline(rl, input, *prompt,
					 last_cmd_status_res,
					 last_cmd_status_s, input_method,
					 context, base_context);
	if (stat == 0)
		return (1);
	if (stat == 2)
	{
		if (*input_method != INP_READLINE)
			*should_exit = true;
		return (2);
	}
	return (0);
}

bool ends_with_bs_nl(t_dyn_str s)
{
	size_t i;
	bool unterminated;

	if (s.len == 0)
		return (false);
	i = s.len;
	unterminated = false;
	if (s.buff[--i] != '\n')
		return (false);
	while (i > 0)
	{
		i--;
		if (s.buff[i] == '\\')
			unterminated = !unterminated;
		else
			break;
	}
	return (unterminated);
}

static void extend_bs(t_rl *rl, t_dyn_str *input, t_status *last_cmd_status_res, char **last_cmd_status_s, int *input_method, char **context, char **base_context, int *should_exit)
{
	char *prompt;

	while (ends_with_bs_nl(*input))
	{
		dyn_str_pop(input);
		dyn_str_pop(input);
		prompt = ft_strdup("> ");
		if (readline_cmd(rl, &prompt, input, last_cmd_status_res, last_cmd_status_s, input_method, context, base_context, should_exit))
			return;
	}
}

void get_more_tokens(t_deque *tt, t_rl *rl,
					 char **prompt, t_dyn_str *input, t_status *last_cmd_status_res,
					 char **last_cmd_status_s, int *input_method,
					 char **context, char **base_context, int *should_exit)
{
	int stat;
	char *curr_prompt;
	char *next_prompt;

	while (*prompt)
	{
		curr_prompt = *prompt;
		stat = readline_cmd(rl, prompt, input, last_cmd_status_res, last_cmd_status_s, input_method, context, base_context, should_exit);
		if (stat)
		{
			free(curr_prompt);
			return;
		}
		next_prompt = (extend_bs(rl, input, last_cmd_status_res, last_cmd_status_s, input_method, context, base_context, should_exit), tokenizer(input->buff, tt));
		free(curr_prompt);
		if (next_prompt)
			*prompt = ft_strdup(next_prompt);
		else
			*prompt = 0;
	}
}
