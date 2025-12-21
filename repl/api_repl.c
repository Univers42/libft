/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   api_repl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 23:26:55 by marvin            #+#    #+#             */
/*   Updated: 2025/12/21 02:18:52 by marvin           ###   ########.fr       */
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

char *getpid_hack(void)
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
	dyn_str_append_fd(&file, fd);
	close(fd);
	char *cstr = ft_strdup(file.buff ? file.buff : "");
	
	free(file.buff);
	file.buff = NULL;

	temp = ft_split(cstr, ' ');
	free(cstr);
	
	if (!temp || !temp[0])
	{
		if (temp)
			free_tab(temp);
		return ft_strdup("0");
	}
	ret = ft_strdup(temp[0]);
	free_tab(temp);
	return (ret);
}


void init_cwd(t_dyn_str *_cwd)
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


void repl(t_repl_config *conf, char **argv, char **envp)
{
	t_api_readline meta;

	init_repl(&meta, argv, envp, conf);
	
	while (!meta.should_exit)
	{
		dyn_str_init(&meta.input);
		get_g_sig()->should_unwind = 0;
		parse_input(&meta);
		
		{
			t_repl_config *rc = get_repl_config();
			if ((rc == NULL || rc->enable_history))
				manage_history_input(&meta.hist, &meta.rl, &meta.input);
		}
		buff_readline_reset(&meta.rl);
		free(meta.input.buff);
		meta.input = (t_dyn_str){0};
	}
	if (conf && conf->restore_signals)
		conf->restore_signals();
	else if (!conf || conf->handle_signals)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
	free_env(&meta.env);
	free_all_state(&meta);
	forward_exit_status(meta.last_cmd_status_res);
}
