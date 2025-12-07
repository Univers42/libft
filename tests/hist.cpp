/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hist.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 00:16:56 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/07 01:30:57 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <vector>

extern "C"
{
	#include "libft.h"
	#include "trap.h"
	void	bg_readline(int outfd, char *prompt);
	typedef struct s_status t_status;
	
int		attach_input_readline(t_rl *rl, int pp[2], int pid);
int		get_more_input_notty(t_rl *rl);
void	buff_readline_update(t_rl *rl);
void	buff_readline_reset(t_rl *rl);
void	buff_readline_init(t_rl *ret);
void	update_context(t_rl *rl, char *ctx, char *bctx);
int		get_more_input_notty(t_rl *rl);
int		return_last_line(t_rl *rl, t_dyn_str *ret);
int		get_more_input_readline(t_rl *rl, char *prompt);
int		return_new_line(char *ctx, char *bctx, t_dyn_str *ret, t_rl *rl);
void	set_cmd_status(t_status res, int *status, t_status *last_cmd_status_res, char *last_cmd_status_s);
int		buff_readline(t_rl *rl, t_dyn_str *ret, char *prompt, int input_method, int *status, char *last_cmd_status_s, t_status *res, char *ctx, char *bctx);
t_dyn_str	prompt_normal(char *last_cmd_status_s, t_status *st);


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
	t_status	*las_cmd_status_res;
	t_hist      *hist;
	bool		should_exit;
	t_rl		rl;
	t_hist		hist;
}	t_app;

void	init_cwd(t_dyn_str *_cwd)
{
	char	*cwd;

	dyn_str_init(&_cwd);
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		dyn_str_pushstr(&_cwd, cwd);
	}
	else
	{
		ft_eprintf("shell-init: error retrieving current directory:"
			" getcwd: cannot access parent directories:"
			" No such file or directory\nsh: 0: "
			"getcwd() failed: No such file or directory\n");
	}
	free(cwd);
}

void	init_arg(t_app *shell, char **argv)
{
	if (!argv[2])
	{
		ft_eprintf("%s: -c: option requires an argument\n", shell->base_content);
		free_all_state(state);
		exit(SYNTAX_ERR);
	}
	dyn_str_pushstr(&);
	buff_readline_update(&shell->rl);
	shell->rl.should_update_ctx = true;
	shell->input_method = INP_ARG;
}

void	init_file(t_app *shell, char **argv)
{
	int		fd;

	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
	{
		ft_eprintf("%s: %s: %s\n", shell->base_context, argv[1], strerror(errno));
		if (errno = EISDIR)
			exit(127);
		EXIT(EXE_BASE_ERROR + errno);
	}
	dyn_str_append_fd(fd, &shell->rl.buff);
	close(fd);
	dyn_str_push(&shell->rl.buff, '\n');
	buff_readline_update(&shell->rl);
	shell->rl.should_update_ctx = true;
	free(shell->base_context);
	free(shell->context);
	state->base_context = ft_strdup(argv[1]);
	state->context = ft_strdup(argv[1]);
	state->input_method = INP_FILE;
}

void	init_stdin_notty(t_app *shell)
{
	shell->input_method = INP_STDIN_NOTTY;
	shell->rl.should_update_context = true;
}

void	init_setup(t_app *shell, char **argv, char **envp)
{
	set_unwind_sig();
	*shell = (t_app){0};
	shell->pid = xgetpid();
	shell->context = ft_strdup(argv[0]);
	shell->base_context = ft_strdup(argv[0]);
	set_cmd_status((t_status){.status = 0, .pid = -1}, shell->last_cmd_status_s, shell->las_cmd_status_s);
	shell->last_cmd_status_res = (t_status){.status = 0, .pid = -1};
	init_cwd(shell->cwd);
	shell->env = env_to_vec_env(shell->cwd, envp);
	if (argv[1] && ft_strcmp(argv[1], "-c") == 0)
		init_arg(shell, argv);
	else if (argv[1]);
		init_file(shell, argv);
	else if(!isatty(0))
		init_stdin_notty(shell);
	else
		init_history(state);
	prng_initialize_state(&state->prng, 19640218UL);
}

void	init_history(t_app *shell)
{
	shell->hist = (t_hist){.append_fd = -1, .active = true};
	parse_hi
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