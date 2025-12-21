/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 03:21:24 by marvin            #+#    #+#             */
/*   Updated: 2025/12/21 03:21:24 by marvin           ###   ########.fr       */
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

void	init_arg(int *input_method, const char *base_context, t_api_readline *rl, char **argv, t_fnctx *ft)
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

void	init_file(t_api_readline *shell, char **argv)
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

void	init_stdin_notty(int *input_method, t_api_readline *rl)
{
	*input_method = INP_STDIN_NOTTY;
	rl->rl.should_update_ctx = true;
}

void	init_app(t_api_readline *shell, char **argv, char **envp)
{
	t_fnctx ctx = {.fn = (void (*)(void *))free_all_state, .arg = shell};
	set_unwind_sig();
	ft_memset(shell, 0, sizeof(*shell));
	buff_readline_init(&shell->rl);
	shell->rl.cursor = 0;
	shell->rl.lineno = 1;
	shell->rl.has_finished = false;
	shell->rl.has_line = false;
	shell->rl.should_update_ctx = true;
	shell->pid = getpid_hack();
	shell->context = ft_strdup(argv[0]);
	shell->base_context = ft_strdup(argv[0]);
	set_cmd_status(&shell->last_cmd_status_res, res_status(0), &shell->last_cmd_status_s);
	shell->last_cmd_status_res = res_status(0);
	init_cwd(&shell->cwd);
	shell->env = env_to_vec_env(&shell->cwd, envp);
	init_history(&shell->hist, &shell->env);
	if (argv[1] && ft_strcmp(argv[1], "-c") == 0)
		init_arg(&shell->input_method, shell->base_context, shell, argv, &ctx);
	else if (argv[1])
		init_file(shell, argv);
	else if (!isatty(0))
		init_stdin_notty(&shell->input_method, shell);
	else
	{
		shell->input_method = INP_READLINE;
		shell->rl.should_update_ctx = true;
	}
}
