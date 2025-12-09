/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_more_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 11:43:56 by anddokhn          #+#    #+#             */
/*   Updated: 2025/12/09 00:42:12 by dlesieur         ###   ########.fr       */
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

	stat = xreadline(rl, input, *prompt,
						last_cmd_status_res,
						last_cmd_status_s, input_method,
						context, base_context);
	free(*prompt);
	*prompt = 0;
	if (stat == 0)
	{
		return (1);
	}
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
	char	looking_for;

	looking_for = *(char*)tt->ctx;
	while (*prompt)
	{
		stat = readline_cmd(rl, prompt, input, last_cmd_status_res, last_cmd_status_s, input_method, context, base_context, should_exit);
		if (stat == 1)
		{
			if (looking_for && input->len)
				ft_eprintf("%s: unexpected EOF while looking for "
							"matching `%c'\n",
						   *context, looking_for);
			else if (input->len)
				ft_eprintf("%s: syntax error: unexpected end of file\n", *context);
			if (*input_method == INP_READLINE)
				ft_eprintf("exit\n");
			if (!last_cmd_status_res->status && input->len)
				set_cmd_status(last_cmd_status_res, (t_status){.status = SYNTAX_ERR}, last_cmd_status_s);
			*should_exit = true;
		}
		if (stat)
			return;
		*prompt = (extend_bs(rl, input, last_cmd_status_res, last_cmd_status_s, input_method, context, base_context, should_exit), tokenizer(input->buff, tt));
		if (*prompt)
			*prompt = ft_strdup(*prompt);
	}
}
