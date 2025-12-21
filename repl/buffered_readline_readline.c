/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffered_readline_readline.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 23:27:02 by marvin            #+#    #+#             */
/*   Updated: 2025/12/21 23:33:08 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"
#include <stdio.h>
#include <signal.h>
#include <readline/readline.h>
#include <sys/wait.h>
#include <unistd.h>
#include "ft_stdio.h"

#define OLD_SIGINT 0
#define OLD_SIGQUIT 1
#define IGN 2

/* exit codes:
 *
 * 0 - verything ok
 *
 * 1 - ctrl-d
 *
 * 2 - ctrl-c / ctrl-\
 */
void	bg_readline(int outfd, char *prompt)
{
	char	*ret;

	rl_instream = stdin;
	rl_outstream = stderr;
	/* prevent readline from installing signal handlers that kill the pgrp */
	extern int rl_catch_signals;
	extern int rl_catch_sigwinch;
	rl_catch_signals = 0;
	rl_catch_sigwinch = 0;
	ret = readline(prompt);
	if (!ret)
	{
		close(outfd);
		exit(1);
	}
	write_to_file(ret, outfd);
	close(outfd);
	exit(0);
}

static void	readline_bg_signals(void)
{
	/* ignore Ctrl+C/\\ in the child; parent handles signals */
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

int	attach_input_readline(t_rl *l, int pp[2], int pid)
{
	int	status;

	close(pp[1]);
	dyn_str_append_fd(&l->str, pp[0]);
	buff_readline_update(l);
	close(pp[0]);
	while (1)
		if (waitpid(pid, &status, 0) != -1)
			break ;
	if (WIFSIGNALED(status))
	{
		ft_eprintf("\n");
		return (2);
	}
	return (WEXITSTATUS(status));
}

static int	attach_and_restore_readline_parent(t_rl *l, int pp[2], int pid,
					struct sigaction sigint[])
{
	int				ret;
	t_repl_config	*rc;

	rc = get_repl_config();
	ret = attach_input_readline(l, pp, pid);
	if (rc == NULL || rc->handle_signals)
	{
		sigaction(SIGINT, &sigint[OLD_SIGINT], NULL);
		sigaction(SIGQUIT, &sigint[OLD_SIGQUIT], NULL);
	}
	return (ret);
}

int	get_more_input_readline(t_rl *l, char *prompt)
{
	int					pp[2];
	int					pid;
	struct sigaction	sigint[3];
	t_repl_config		*rc;

	rc = get_repl_config();
	if (rc == NULL || rc->handle_signals)
	{
		ft_memset(&sigint[IGN], 0, sizeof(sigint[IGN]));
		sigint[IGN].sa_handler = SIG_IGN;
		sigemptyset(&sigint[IGN].sa_mask);
		sigaction(SIGINT, &sigint[IGN], &sigint[OLD_SIGINT]);
		sigaction(SIGQUIT, &sigint[IGN], &sigint[OLD_SIGQUIT]);
	}
	if (pipe(pp))
		critical_error_errno_context("pipe");
	pid = fork();
	if (pid == 0)
		(readline_bg_signals(), close(pp[0]), bg_readline(pp[1], prompt));
	else if (pid < 0)
		critical_error_errno_context("fork");
	else
		return (attach_and_restore_readline_parent(l, pp, pid, sigint));
	return (ft_assert("Unreachable" == 0), 0);
}
