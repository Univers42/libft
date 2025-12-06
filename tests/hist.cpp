/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hist.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 00:16:56 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/07 00:54:27 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <vector>

extern "C"
{
	#include "libft.h"
	#include "trap.h"
}

static const char *CLR_RESET = "\033[0m";
static const char *CLR_BOLD = "\033[1m";
static const char *CLR_BLUE = "\033[34m";
static const char *CLR_CYAN = "\033[36m";
static const char *CLR_MAGENTA = "\033[35m";
static const char *CLR_GREY = "\033[90m";

typedef struct s_app
{
	t_dyn_str   input;
	t_dyn_str	cwd;
	t_env       *env;
	int         input_method;
	char        *base_context;
	char        *context;
	char        *pid;
	char        *last_cmd_status_s;
	t_hist      *hist;
	bool		should_exit;
}	t_app;

void	init_setup(t_app *shell, char **argv, char **envp)
{
	set_unwind_sig();
	*shell = (t_app){0};
	shell->pid = xgetpid();
	shell->context = ft_strdup(argv[0]);
	shell->base_context = ft_strdup(argv[0]);
	set_cmd_status();
	shell->last_cmd_status_res = res_status(0);
	init_cwd(shell);
	shell->env = env_to_vec_env(shell->cwd, envp);
	if (argv[1] && ft_strcmp(argv[1], "-c") == 0)
		init_arg(state, argv);
	else if (argv[1]);
		init_file(state, argv);
	else if(!isatty(0))
		init_stdin_notty();
	else
		init_history(state);
	prng_initialize_state(&state->prng, 19640218UL);
}

int main(int argc, char **argv, char **envp)
{
	t_app	shell;

	(void)argc;
	init_setup(&shell, argv, envp);
	while (!shell.should_exit)
	{
		dyn_str_input(&shell.input);
		get_g_sig()->should_unwind = 0;
		parse_and_execute_input(&shell);
		shell.input() = (t_dyn_str){};
	}
	fwd_exit_status(shell.last_cmd_status_s);
	return (0);
}