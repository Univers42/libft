/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hist.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 00:16:56 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/21 03:26:55 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <unistd.h>

extern "C"
{
#include "libft.h"
#include "ft_stddef.h"
#include "ft_readline.h"
#include "parser.h"
#include "ds.h"
}

const char *CLR_RESET = "\033[0m";
const char *CLR_BOLD = "\033[1m";
const char *CLR_BLUE = "\033[34m";
const char *CLR_CYAN = "\033[36m";
const char *CLR_MAGENTA = "\033[35m";
const char *CLR_GREEN = ASCII_GREEN;

void parse_input(t_api_readline *shell)
{
	char *prompt;
	t_parse parser;

	parser = (t_parse){.st = ST_INIT, .stack = {}};
	prompt = prompt_normal(&shell->last_cmd_status_res, &shell->last_cmd_status_s).buff;
	{
		t_getmore_ctx gm_ctx = {
			.prompt = &prompt,
			.input = &shell->input,
			.last_cmd_status_res = &shell->last_cmd_status_res,
			.last_cmd_status_s = &shell->last_cmd_status_s,
			.input_method = &shell->input_method,
			.context = &shell->context,
			.base_context = &shell->base_context,
			.should_exit = &shell->should_exit,
			.out_tokens = NULL
		};
		get_more_tokens(&shell->rl, &gm_ctx);
	}
	if (get_g_sig()->should_unwind)
		set_cmd_status(&shell->last_cmd_status_res, (t_status){.status = CANCELED, .pid = -1, .c_c = true}, &shell->last_cmd_status_s);
	free(parser.stack.buff);
	parser.stack = (t_vec){};
	shell->should_exit |= ((get_g_sig()->should_unwind && shell->input_method != INP_READLINE) || shell->rl.has_finished);
}

static void print_welcome_once(void)
{
	const char *user = getenv("USER");
	if (!user)
		user = "friend";
	printf("\n\033[1;36m🔷 Welcome %s to UNICODE-SHELL (sh42)\033[0m\n", user);
	printf("\033[0;37mThis shell is a work in progress — improvements are ongoing.\n");
	printf("Contribute or report issues: \033[1;34mhttps://github.com/Univers42/sh42.git\033[0m\n\n");
	fflush(stdout);
}

int	main(int argc, char **argv, char **envp)
{
	t_api_readline shell;

	(void)argc;
	init_app(&shell, argv, envp);
	print_welcome_once();
	while (!shell.should_exit)
	{
		dyn_str_init(&shell.input);
		get_g_sig()->should_unwind = 0;
		parse_input(&shell);
		if (shell.rl.cursor > 1)
			manage_history(&shell.hist, &shell.rl);
		buff_readline_reset(&shell.rl);
		free(shell.input.buff);
		shell.input = (t_dyn_str){};
	}
	free_env(&shell.env);
	free_all_state(&shell);
	forward_exit_status(shell.last_cmd_status_res);
}