/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffered_readline.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 22:10:06 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/05 23:21:42 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "readline.h"
#include <readline/readline.h>
#include "ft_stdio.h"
#include "trap.h"
#include "ft_memory.h"
#include "ft_string.h"
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void readline_bg_signals(void);
int write_file(const char *s, int fd);

void rlupdate(t_rl *line)
{
	if (!line || !line->str)
		return;
	line->has_line = line->cursor < line->str->len;
}

void rlreset(t_rl *line)
{
	if (!line || !line->str)
		return;
	if (line->cursor < line->str->len)
		ft_memmove(line->str->buff, line->str->buff + line->cursor,
				   line->str->len - line->cursor);
	if (line->cursor <= line->str->len)
		line->str->len -= line->cursor;
	if (line->str->buff)
		line->str->buff[line->str->len] = 0;
	line->cursor = 0;
	rlupdate(line);
}

void update_ctx(t_rl *line, char **ctx, const char *base_ctx)
{
	if (!line || !ctx || !base_ctx || line->should_update_ctx)
		return;
	free(*ctx);
	*ctx = NULL;
	ft_asprintf(ctx, "%s: Line %d", base_ctx, line->lineno);
}

int get_more_input_notty(t_rl *rl)
{
	char buff[BUF_SIZE * 4];
	int ret;
	int status;

	status = 1;
	set_unwind_sig_norestart();
	while (ST_SCANNING)
	{
		ret = read(0, buff, sizeof(buff));
		if (ret < 0 && errno == EINTR)
			status = 2;
		if (ret == 0)
			rl->has_finished = true;
		if (ret == 0)
			dyn_str_pushstr(rl->str, "\n");
		if (ret <= 0)
			break;
		status = 0;
		dyn_str_pushnstr(rl->str, buff, ret);
		if (ft_strnchr(buff, '\n', ret))
			break;
	}
	set_unwind_sig();
	rlupdate(rl);
	return (status);
}

void bg_readline(int outfd, const char *prompt)
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
	write_file(ret, outfd);
	close(outfd);
	exit(0);
}

int attach_input_readline(t_dyn_str *str, int pp[2], int pid)
{
	int status;

	close(pp[1]);
	dyn_str_append_fd(pp[0], str);
	close(pp[0]);
	while (ST_SCANNING)
		if (waitpid(pid, &status, 0) != -1)
			break;
	if (WIFSIGNALED(status))
		return (ft_eprintf("\n"), 2);
	return (WEXITSTATUS(status));
}

int get_more_input_readline(t_dyn_str *str, const char *prompt)
{
	int pp[2];
	int pid;

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
		return (attach_input_readline(str, pp, pid));
	ft_assert("Unreachable" == 0);
	return (0);
}