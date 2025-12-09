/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hist.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 00:16:56 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/09 02:12:07 by dlesieur         ###   ########.fr       */
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

// static const char *CLR_RESET = "\033[0m";
// static const char *CLR_BOLD = "\033[1m";
// static const char *CLR_BLUE = "\033[34m";
// static const char *CLR_CYAN = "\033[36m";
// static const char *CLR_MAGENTA = "\033[35m";
// static const char *CLR_GREEN = GREEN_TERM;
typedef struct s_app
{
	t_dyn_str input;
	t_dyn_str cwd;
	t_vec env;
	t_hist hist;
	int input_method;
	char *base_context;
	char *context;
	char *pid;
	char *last_cmd_status_s;
	t_status last_cmd_status_res;
	bool should_exit;
	t_rl rl;
} t_app;

static t_status res_status(int status)
{
	return ((t_status){.status = status, .pid = -1, .c_c = false});
}

static void free_all_state(t_app *shell)
{
	free(shell->input.buff);
	shell->input = (t_dyn_str){};
	free(shell->last_cmd_status_s);
	free(shell->pid);
	free(shell->context);
	free(shell->base_context);
	shell->context = 0;
	shell->base_context = 0;
	free(shell->rl.str.buff);
	free_hist(&shell->hist);
	free(shell->cwd.buff);
}

static void init_cwd(t_dyn_str *_cwd)
{
	char *cwd;

	dyn_str_init(_cwd);
	cwd = getcwd(NULL, 0);
	if (!cwd)
		cwd = getenv("PWD");
	if (cwd)
		dyn_str_pushstr(_cwd, cwd);
	else
		ft_eprintf("Shell-init: getcwd failed: %s\n", strerror(errno));
	free(cwd);
}

static void init_arg(int *input_method, const char *base_context, t_rl *rl, char **argv, t_fnctx *ft)
{
	if (!argv[2])
	{
		ft_eprintf("%s: -c: option requires an argument\n", base_context);
		if (ft && ft->fn)
			ft->fn(ft->arg);
		exit(SYNTAX_ERR);
	}
	dyn_str_pushstr(&rl->str, argv[2]);
	buff_readline_update(rl);
	rl->should_update_ctx = true;
	*input_method = INP_ARG;
}

static void init_file(t_app *shell, char **argv)
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
	dyn_str_append_fd(fd, &shell->rl.str);
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

static void init_stdin_notty(int *input_method, t_rl *rl)
{
	*input_method = INP_STDIN_NOTTY;
	rl->should_update_ctx = true;
}

char	*getpid_hack(void)
{
	int			fd;
	t_dyn_str	file;
	char		*ret;
	char		**temp;
	const char	*err = "CAnnot get PID.";
	fd = open("/proc/self/stat", O_RDONLY);
	if (fd < 0)
	{
		warning_error(err);
		return (0);
	}
	dyn_str_init(&file);
	dyn_str_append_fd(fd, &file);
	close(fd);
	temp = ft_split(file.buff, ' ');
	free(file.buff);
	ret = ft_strdup(temp[0]);
	free_tab(temp);
	return (ret);
}

static void init_app(t_app *shell, char **argv, char **envp)
{
	t_fnctx ctx = {.fn = (void (*)(void *))free_all_state, .arg = shell};

	set_unwind_sig();
	*shell = (t_app){
		.input = {},
		.cwd = {},
		.env = {},
		.hist = {},
		.input_method = 0,
		.base_context = NULL,
		.context = NULL,
		.pid = NULL,
		.last_cmd_status_s = NULL,
		.last_cmd_status_res = {},
		.should_exit = false,
		.rl = {}};
	shell->pid = getpid_hack();
	shell->context = ft_strdup(argv[0]);
	shell->base_context = ft_strdup(argv[0]);
	set_cmd_status(&shell->last_cmd_status_res, res_status(0), &shell->last_cmd_status_s);
	shell->last_cmd_status_res = res_status(0);
	init_cwd(&shell->cwd);
	shell->env = env_to_vec_env(&shell->cwd, envp);
	if (argv[1] && ft_strcmp(argv[1], "-c") == 0)
		init_arg(&shell->input_method, shell->base_context, &shell->rl, argv, &ctx);
	else if (argv[1])
		init_file(shell, argv);
	else if (!isatty(0))
		init_stdin_notty(&shell->input_method, &shell->rl);
	else
		init_history(&shell->hist, &shell->env);
}

static void parse_input(t_app *shell)
{
	t_deque tt;
	char *prompt;
	t_parse parser;
	char *looking_for;

	looking_for = (char *)malloc(sizeof(char));
	*looking_for = '\0';
	parser = (t_parse){.st = ST_INIT, .stack = {}};
	prompt = prompt_normal(&shell->last_cmd_status_res, &shell->last_cmd_status_s).buff;
	deque_init(&tt, 64, sizeof(t_token), (void *)looking_for);
	get_more_tokens(&tt, &shell->rl, &prompt, &shell->input, &shell->last_cmd_status_res, &shell->last_cmd_status_s,
					&shell->input_method, &shell->context, &shell->base_context, (int *)&shell->should_exit);
	if (get_g_sig()->should_unwind)
		set_cmd_status(&shell->last_cmd_status_res, (t_status){.status = CANCELED, .pid = -1, .c_c = true}, &shell->last_cmd_status_s);
	free(parser.stack.buff);
	parser.stack = (t_vec){};
	free(tt.buf);
	free(looking_for);
	shell->should_exit |= ((get_g_sig()->should_unwind && shell->input_method != INP_READLINE) || shell->rl.has_finished);
}

int main(int argc, char **argv, char **envp)
{
	t_app shell;

	(void)argc;
	init_app(&shell, argv, envp);
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