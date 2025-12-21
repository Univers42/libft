/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl_custom.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 20:34:00 by marvin            #+#    #+#             */
/*   Updated: 2025/12/20 20:34:00 by marvin           ###   ########.fr       */
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
}

const char *CLR_RESET = "\033[0m";
const char *CLR_BOLD = "\033[1m";
const char *CLR_BLUE = "\033[34m";
const char *CLR_CYAN = "\033[36m";
const char *CLR_MAGENTA = "\033[35m";
const char *CLR_GREEN = ASCII_GREEN;

 void init_arg(int *input_method, const char *base_context, t_api_readline *rl, char **argv, t_fnctx *ft)
{
	if (!argv[2])
	{
		ft_eprintf("%s: -c: option requires an argument\n", base_context);
		if (ft && ft->fn)
			ft->fn(ft->arg);
		exit(SYNTAX_ERR);
	}
	dyn_str_pushstr(&rl->rl.str, argv[2]);
	buff_readline_update(&rl->rl);
	rl->rl.should_update_ctx = true;
	*input_method = INP_ARG;
}

 void init_file(t_api_readline *shell, char **argv)
{
	int fd;

	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
	{
		ft_eprintf("%s: %s: %s\n", shell->base_context, argv[1], strerror(errno));
		free_all_state(shell);
		if (errno == EISDIR)
			exit(127);
		exit(EXE_BASE_ERROR + errno);
	}
	dyn_str_append_fd(&shell->rl.str, fd);
	close(fd);
	dyn_str_push(&shell->rl.str, ESC_N);
	buff_readline_update(&shell->rl);
	shell->rl.should_update_ctx = true;
	free(shell->base_context);
	free(shell->context);
	shell->base_context = ft_strdup(argv[1]);
	shell->context = ft_strdup(argv[1]);
	shell->input_method = INP_FILE;
}

 void init_stdin_notty(int *input_method, t_api_readline *rl)
{
	*input_method = INP_STDIN_NOTTY;
	rl->rl.should_update_ctx = true;
}

// Custom prompt function (C linkage)
extern "C" t_dyn_str my_custom_prompt(t_status *st, char **st_s)
{
	(void)st_s;
	t_dyn_str p;
	dyn_str_init(&p);
	dyn_str_pushstr(&p, "\033[1;36m[MY-REPL]\033[0m ");
	if (st && st->status != 0)
	{
		dyn_str_pushstr(&p, "\033[1;31mERR:");
		char buf[16];
		snprintf(buf, sizeof(buf), "%d", st->status);
		dyn_str_pushstr(&p, buf);
		dyn_str_pushstr(&p, "\033[0m ");
	}
	return p;
}

// Optional: Unicode prompt example
extern "C" t_dyn_str my_unicode_prompt(t_status *st, char **st_s)
{
	(void)st_s;
	t_dyn_str p;
	dyn_str_init(&p);
	// emoji + unicode characters are supported; wrapping of ANSI is handled by REPL config
	dyn_str_pushstr(&p, "🔷 ");
	dyn_str_pushstr(&p, "\033[1;34m"); // colored label (will be wrapped if enabled)
	dyn_str_pushstr(&p, "UNICODE-SHELL");
	dyn_str_pushstr(&p, "\033[0m ");
	if (st && st->status != 0)
	{
		dyn_str_pushstr(&p, "(err ");
		char buf[16];
		snprintf(buf, sizeof(buf), "%d", st->status);
		dyn_str_pushstr(&p, buf);
		dyn_str_pushstr(&p, ") ");
	}
	return p;
}

extern "C" void repl(t_repl_config *conf, char **argv, char **envp);

int main(int argc, char **argv, char **envp)
{
	(void)argc; // Silence unused parameter warning

	// Setup config for custom prompt and behavior
	t_repl_config config = {};
	// assign one prompt generator (pick either)
	config.prompt_gen = my_unicode_prompt;

	// Control features:
	config.enable_vcs = false;           // disable git queries in default prompt (faster)
	config.enable_chrono = true;         // show chrono in prompt
	config.wrap_prompt_nonprint = true;  // automatically wrap ANSI escapes with \001...\002
	config.enable_history = true;        // enable history management
	config.handle_signals = true;		 // enable default REPL signal behavior
	config.input_method = INP_READLINE;
	config.base_context = argv[0];

	/* Examples: override specific signals (optional)
	   - Use SIG_IGN, SIG_DFL, or provide a function pointer (t_sig_handler) here.
	   - Defaults: leave entries NULL to keep REPL default behavior.
	*/
	config.sig_actions[SIGINT]  = SIG_IGN; // ignore Ctrl+C
	config.sig_actions[SIGQUIT] = SIG_DFL; // Ctrl+\ default
	//config.sig_actions[SIGTERM] = my_custom_term_handler; // custom handler

	// t_repl_config has many fields; leave env/context NULL to let REPL initialize them
	repl(&config, argv, envp);
	return 0;
}