/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffered_readline_readline.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 23:27:02 by marvin            #+#    #+#             */
/*   Updated: 2025/12/20 23:27:05 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "ft_readline.h"
#include <stdio.h>
#include <signal.h>
#include <readline/readline.h>
#include <sys/wait.h>
#include <unistd.h>
#include "ft_stdio.h"

/* exit codes:
 *
 * 0 - verything ok
 *
 * 1 - ctrl-d
 *
 * 2 - ctrl-c / ctrl-\
 */
void bg_readline(int outfd, char *prompt)
{
	char *ret;

	rl_instream = stdin;
	rl_outstream = stderr;
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

int attach_input_readline(t_rl *l, int pp[2], int pid)
{
	int status;

	close(pp[1]);
	dyn_str_append_fd(&l->str, pp[0]);
	buff_readline_update(l);
	close(pp[0]);
	while (1)
		if (waitpid(pid, &status, 0) != -1)
			break;
	if (WIFSIGNALED(status))
	{
		ft_eprintf("\n");
		return (2);
	}
	return (WEXITSTATUS(status));
}

static void	readline_bg_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
}

int get_more_input_readline(t_rl *l, char *prompt)
{
	int pp[2];
	int pid;
	struct sigaction old_sigint, old_sigquit, ign;
	t_repl_config *rc;

	rc = get_repl_config();
	
	if (rc == NULL || rc->handle_signals)
	{
		ft_memset(&ign, 0, sizeof(ign));
		ign.sa_handler = SIG_IGN;
		sigemptyset(&ign.sa_mask);
		sigaction(SIGINT, &ign, &old_sigint);
		sigaction(SIGQUIT, &ign, &old_sigquit);
	}

	if (pipe(pp))
		critical_error_errno_context("pipe");
	pid = fork();
	if (pid == 0)
	{
		
		readline_bg_signals();
		close(pp[0]);
		bg_readline(pp[1], prompt);
	}
	else if (pid < 0)
		critical_error_errno_context("fork");
	else
	{
		int ret = attach_input_readline(l, pp, pid);
		
		if (rc == NULL || rc->handle_signals)
		{
			sigaction(SIGINT, &old_sigint, NULL);
			sigaction(SIGQUIT, &old_sigquit, NULL);
		}
		return (ret);
	}
	ft_assert("Unreachable" == 0);
	return (0);
}
