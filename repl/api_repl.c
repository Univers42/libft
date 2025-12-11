/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   api_repl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 20:52:41 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/11 11:49:56 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"
#include <stdbool.h>
#include "ft_stdio.h"
#include "ipc.h"
#include "var.h"
#include "ipc.h"

static void free_all_state(t_stream_dft_data	*shell)
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

static char *getpid_hack(void)
{
	int fd;
	t_dyn_str file;
	char *ret;
	char **temp;
	const char *err = "CAnnot get PID.";
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

static t_status res_status(int status)
{
	return ((t_status){.status = status, .pid = -1, .c_c = false});
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

static void init_repl(t_stream_dft_data *meta, char **argv, char **envp, t_repl_config *conf)
{
	t_fnctx ctx = {.fn = (void (*)(void *))free_all_state, .arg = meta};

	(void)ctx;
	(void)conf;
	set_unwind_sig();
	*meta = (t_stream_dft_data){};
	meta->pid = getpid_hack();
	meta->context = ft_strdup(argv[0]);
	meta->base_context = ft_strdup(argv[0]);
	set_cmd_status(&meta->last_cmd_status_res, res_status(0), &meta->last_cmd_status_s);
	meta->last_cmd_status_res = res_status(0);
	init_cwd(&meta->cwd);
	meta->env = env_to_vec_env(&meta->cwd, envp);
	init_history(&meta->hist, &meta->env);
	
}

static void parse_input(t_stream_dft_data *meta)
{
	char	*prompt;
	t_parse	parser;

	parser = (t_parse){.st = ST_INIT, .stack = {}};
	prompt = prompt_normal(&meta->last_cmd_status_res, &meta->last_cmd_status_s).buff;
	get_more_tokens(&meta->rl, &prompt, &meta->input, &meta->last_cmd_status_res, &meta->last_cmd_status_s, &meta->input_method, &meta->context, &meta->base_context, &meta->should_exit);
	if (get_g_sig()->should_unwind)
		set_cmd_status(&meta->last_cmd_status_res, (t_status){.status = CANCELED, .pid = -1, .c_c = true}, &meta->last_cmd_status_s);
	free(parser.stack.buff);
	parser.stack = (t_vec){};
	meta->should_exit |= (((get_g_sig()->should_unwind && meta->input_method != INP_READLINE) || meta->rl.has_finished));
}

void    repl(t_repl_config *conf, char **argv, char **envp)
{
	t_stream_dft_data	meta;

	init_repl(&meta, argv, envp, conf);
	while (meta.should_exit)
	{
		dyn_str_init(&meta.input);
		get_g_sig()->should_unwind = 0;
		parse_input(&meta);
		if (meta.rl.cursor > 1)
			manage_history(&meta.hist, &meta.rl);
		buff_readline_reset(&meta.rl);
		free(meta.input.buff);
		meta.input = (t_dyn_str){};
	}
	free_env(&meta.env);
	free_all_state(&meta);
	forward_exit_status(meta.last_cmd_status_res);
}