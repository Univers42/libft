/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 23:32:50 by marvin            #+#    #+#             */
/*   Updated: 2025/12/20 23:32:50 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"
#include <stdbool.h>
#include "ft_stdio.h"
#include "ipc.h"
#include "var.h"
#include "ipc.h"
#include "lexer.h"
#include <ctype.h> 
#include "ft_wctype.h" 
#include <signal.h> 
#include "trap.h"

static char	*get_wrapped_prompt(t_api_readline *meta)
{
	t_dyn_str	prompt_dyn;
	char		*wrapped;

	if (meta->prompt_gen)
		prompt_dyn = meta->prompt_gen(&meta->last_cmd_status_res,
				&meta->last_cmd_status_s);
	else
		prompt_dyn = prompt_normal(&meta->last_cmd_status_res,
				&meta->last_cmd_status_s);
	wrapped = wrap_nonprint_if_enabled(prompt_dyn.buff);
	free(prompt_dyn.buff);
	prompt_dyn.buff = wrapped;
	wrapped = wrap_combining_if_enabled(prompt_dyn.buff);
	free(prompt_dyn.buff);
	prompt_dyn.buff = wrapped;
	return (prompt_dyn.buff);
}

static void	prepare_and_get_tokens(t_api_readline *meta,
				char **prompt_out, t_deque *tt_out)
{
	t_getmore_ctx	gm_ctx;

	ensure_locale();
	*prompt_out = get_wrapped_prompt(meta);
	deque_init(tt_out, 64, sizeof(t_token), NULL);
	{
		gm_ctx = (t_getmore_ctx){
			.prompt = prompt_out,
			.input = &meta->input,
			.last_cmd_status_res = &meta->last_cmd_status_res,
			.last_cmd_status_s = &meta->last_cmd_status_s,
			.input_method = &meta->input_method,
			.context = &meta->context,
			.base_context = &meta->base_context,
			.should_exit = &meta->should_exit,
			.out_tokens = tt_out
		};
		get_more_tokens(&meta->rl, &gm_ctx);
	}
}

static void	finalize_parse(t_api_readline *meta, t_deque *tt, t_parse *parser)
{
	if (get_g_sig()->should_unwind)
		set_cmd_status(&meta->last_cmd_status_res,
			(t_status){.status = CANCELED, .pid = -1, .c_c = true},
			&meta->last_cmd_status_s);
	free(parser->stack.buff);
	parser->stack = (t_vec){0};
	if (tt->cap && tt->buf)
		free(tt->buf);
	meta->should_exit |= ((get_g_sig()->should_unwind
				&& meta->input_method != INP_READLINE)
			|| meta->rl.has_finished);
}

void	parse_input(t_api_readline *meta)
{
	char	*prompt;
	t_parse	parser;
	t_deque	tt;

	parser = (t_parse){.st = ST_INIT, .stack = {0}};
	prepare_and_get_tokens(meta, &prompt, &tt);
	finalize_parse(meta, &tt, &parser);
}
