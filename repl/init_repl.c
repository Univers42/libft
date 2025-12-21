/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_repl.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 23:37:26 by marvin            #+#    #+#             */
/*   Updated: 2025/12/20 23:37:26 by marvin           ###   ########.fr       */
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

static void	setup_signals_default(void)
{
	struct sigaction sa = {0};
	sa.sa_handler = repl_parent_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	set_unwind_sig();
}

static void	init_repl_state(t_api_readline *meta, char **argv, char **envp, t_repl_config *conf)
{
	*meta = (t_api_readline){0};
	buff_readline_init(&meta->rl);
	meta->rl.cursor = 0;
	meta->rl.lineno = 1;
	meta->rl.has_finished = false;
	meta->rl.has_line = false;
	meta->rl.should_update_ctx = true;
	meta->pid = getpid_hack();
	meta->context = ft_strdup(argv[0]);
	meta->base_context = ft_strdup(argv[0]);
	set_cmd_status(&meta->last_cmd_status_res, res_status(0), &meta->last_cmd_status_s);
	meta->last_cmd_status_res = res_status(0);
	init_cwd(&meta->cwd);
	meta->env = env_to_vec_env(&meta->cwd, envp);
	if (!conf || conf->enable_history)
		init_history(&meta->hist, &meta->env);
	else
		meta->hist = (t_hist){0}; 
	set_repl_config(conf);
	set_repl_state(meta);
	meta->prompt_gen = conf && conf->prompt_gen ? conf->prompt_gen : NULL;
}

void init_repl(t_api_readline *meta, char **argv, char **envp, t_repl_config *conf)
{
	t_fnctx ctx = {.fn = (void (*)(void *))free_all_state, .arg = meta};

	(void)ctx;
	(void)conf;
	if (conf && conf->setup_signals)
		conf->setup_signals();
	else if (!conf || conf->handle_signals)
		setup_signals_default();
	init_repl_state(meta, argv, envp, conf);
}

