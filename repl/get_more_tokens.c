/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_more_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 11:43:56 by anddokhn          #+#    #+#             */
/*   Updated: 2025/12/14 01:46:42 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_readline.h"
#include <stdbool.h>

static int readline_cmd(t_rl *rl, char **prompt, t_dyn_str *input, t_status *last_cmd_status_res, char **last_cmd_status_s, int *input_method, char **context, char **base_context, bool *should_exit)
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

static void extend_bs(t_rl *rl, t_dyn_str *input, t_status *last_cmd_status_res, char **last_cmd_status_s, int *input_method, char **context, char **base_context, bool *should_exit)
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

void get_more_tokens(t_rl *rl,
					 char **prompt, t_dyn_str *input, t_status *last_cmd_status_res,
					 char **last_cmd_status_s, int *input_method,
					 char **context, char **base_context, bool *should_exit, t_deque *out_tokens)
{
	int stat;
	char *curr_prompt;
	char *next_prompt;
	t_deque tt;
	bool created_local = false;
	char looking_for = '\0';

	/* Only create a temporary deque if caller did not provide one. */
	if (!out_tokens)
	{
		deque_init(&tt, 64, sizeof(t_token), NULL);
		out_tokens = &tt;
		created_local = true;
	}
	while (prompt && *prompt)
	{
		curr_prompt = *prompt;
		stat = readline_cmd(rl, prompt, input, last_cmd_status_res, last_cmd_status_s, input_method, context, base_context, should_exit);
		if (stat == 1)
		{
			if (looking_for && input->len)
				ft_eprintf("%s: unexpected EOF while looking for "
						   "matching `%c'\n",
						   (context && *context) ? *context : "input", looking_for);
			else if (input->len)
				ft_eprintf("%s: syntax error: unexpected end of file\n", (context && *context) ? *context : "input");
			if (*input_method == INP_READLINE)
				ft_eprintf("exit\n");
			if (!last_cmd_status_res->status && input->len)
				set_cmd_status(last_cmd_status_res, (t_status){.status = SYNTAX_ERR}, last_cmd_status_s);
			*should_exit = true;
		}
		if (stat)
		{
			free(curr_prompt);
			/* If we created a local deque, free its buffer */
			if (created_local && out_tokens && out_tokens->buf)
				free(out_tokens->buf);
			return;
		}
		extend_bs(rl, input, last_cmd_status_res, last_cmd_status_s, input_method, context, base_context, should_exit);
		/* Tokenize into the (possibly caller-provided) deque */
		next_prompt = tokenizer(input->buff, out_tokens);
		free(curr_prompt);
		if (next_prompt)
			*prompt = ft_strdup(next_prompt);
		else
			*prompt = NULL;
	}
	/* Free local deque buffer if we created it here */
	if (created_local && out_tokens && out_tokens->buf)
		free(out_tokens->buf);
}
