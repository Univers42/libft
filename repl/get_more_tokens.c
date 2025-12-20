/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_more_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 11:43:56 by anddokhn          #+#    #+#             */
/*   Updated: 2025/12/20 22:56:04 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdbool.h>
#include "ft_wctype.h" // add for ft_mbrtowc and ft_wcwidth
#include "ft_readline.h" // for new ctx types

static int readline_cmd(t_rl *rl, t_getmore_ctx *ctx)
{
	int stat;
	t_xreadline_ctx xctx;

	/* build small xreadline context from getmore context */
	xctx.ret = ctx->input;
	xctx.prompt = *ctx->prompt;
	xctx.last_cmd_status_res = ctx->last_cmd_status_res;
	xctx.last_cmd_status_s = ctx->last_cmd_status_s;
	xctx.input_method = ctx->input_method;
	xctx.context = ctx->context;
	xctx.base_context = ctx->base_context;

	stat = xreadline(rl, &xctx);
	if (stat == 0)
		return (1);
	if (stat == 2)
	{
		if (*ctx->input_method != INP_READLINE)
			*ctx->should_exit = true;
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

static void extend_bs(t_rl *rl, t_getmore_ctx *ctx)
{
	char *tmp_prompt;
	char *old_prompt;

	while (ends_with_bs_nl(*ctx->input))
	{
		dyn_str_pop(ctx->input);
		dyn_str_pop(ctx->input);
		tmp_prompt = ft_strdup("> ");
		if (!tmp_prompt)
			return;
		/* swap in the temporary prompt for the duration of the call */
		old_prompt = *ctx->prompt;
		*ctx->prompt = tmp_prompt;
		if (readline_cmd(rl, ctx))
		{
			/* restore and free temp before returning */
			*ctx->prompt = old_prompt;
			free(tmp_prompt);
			return;
		}
		/* restore and free temp */
		*ctx->prompt = old_prompt;
		free(tmp_prompt);
	}
}

/* sanitize input buffer: replace invalid UTF-8 leading/continuation with '?' so
   downstream mbrtowc/vis_width/tokenizer do not loop on invalid bytes */
static void sanitize_input_utf8(t_dyn_str *input)
{
	mbstate_t st;
	wchar_t wc;
	size_t r;
	size_t i = 0;
	ft_memset(&st, 0, sizeof(st));
	while (i < input->len)
	{
		r = ft_mbrtowc(&wc, input->buff + i, (size_t)(input->len - i), &st);
		if (r == (size_t)-1)
		{
			/* invalid sequence: replace single byte and continue */
			input->buff[i] = '?';
			ft_memset(&st, 0, sizeof(st));
			i++;
			continue;
		}
		if (r == (size_t)-2)
		{
			/* truncated sequence at end: leave as-is */
			break;
		}
		if (r == 0)
			break;
		i += r;
	}
}

void get_more_tokens(t_rl *rl, t_getmore_ctx *ctx)
{
	int stat;
	char *curr_prompt;
	char *next_prompt;
	t_deque tt;
	t_deque *tokens = NULL;
	bool created_local = false;
	char looking_for = '\0';

	/* Only create a temporary deque if caller did not provide one. */
	if (!ctx->out_tokens)
	{
		deque_init(&tt, 64, sizeof(t_token), NULL);
		tokens = &tt;
		created_local = true;
	}
	else
		tokens = ctx->out_tokens;

	while (ctx->prompt && *ctx->prompt)
	{
		curr_prompt = *ctx->prompt;
		stat = readline_cmd(rl, ctx);
		if (stat == 1)
		{
			if (looking_for && ctx->input->len)
				ft_eprintf("%s: unexpected EOF while looking for matching `%c'\n",
						   (ctx->context && *ctx->context) ? *ctx->context : "input", looking_for);
			else if (ctx->input->len)
				ft_eprintf("%s: syntax error: unexpected end of file\n", (ctx->context && *ctx->context) ? *ctx->context : "input");
			if (*ctx->input_method == INP_READLINE)
				ft_eprintf("exit\n");
			if (!ctx->last_cmd_status_res->status && ctx->input->len)
				set_cmd_status(ctx->last_cmd_status_res, (t_status){.status = SYNTAX_ERR}, ctx->last_cmd_status_s);
			*ctx->should_exit = true;
		}
		if (stat)
		{
			free(curr_prompt);
			if (created_local && tokens && tokens->buf)
				free(tokens->buf);
			return;
		}
		extend_bs(rl, ctx);
		/* sanitize input so invalid UTF-8 bytes don't cause downstream loops */
		sanitize_input_utf8(ctx->input);
		/* Tokenize into the (possibly caller-provided) deque */
		next_prompt = tokenizer(ctx->input->buff, tokens);
		free(curr_prompt);
		if (next_prompt)
			*ctx->prompt = ft_strdup(next_prompt);
		else
			*ctx->prompt = NULL;
	}
	/* Free local deque buffer if we created it here */
	if (created_local && tokens && tokens->buf)
		free(tokens->buf);
}
