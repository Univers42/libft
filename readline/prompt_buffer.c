/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_buffer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 21:02:06 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/08 02:50:32 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_readline.h"
#include "ft_vector.h"
#include "ft_string.h"
#include "ft_memory.h"
#include <fcntl.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include "trap.h"
#include "lexer.h"
#include "var.h"

int write_to_file(char *str, int fd)
{
    ssize_t wrote;
    size_t wrote_total = 0;
    size_t len;

    if (!str || fd < 0)
        return (1);
    len = strlen(str);
    /* write the string AND a terminating newline so the reader can reassemble
       physical lines into a multiline buffer. */
    while (wrote_total < len)
    {
        wrote = write(fd, str + wrote_total, len - wrote_total);
        if (wrote < 0)
        {
            if (errno == EINTR)
                continue;
            return (1);
        }
        wrote_total += (size_t)wrote;
    }
    /* write the explicit newline separator */
    wrote_total = 0;
    while (wrote_total < 1)
    {
        wrote = write(fd, "\n", 1);
        if (wrote < 0)
        {
            if (errno == EINTR)
                continue;
            return (1);
        }
        wrote_total += (size_t)wrote;
    }
    return (0);
}

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

int attach_input_readline(t_rl *rl, int pp[2], int pid)
{
	int status;

	close(pp[1]);
	dyn_str_append_fd(pp[0], &rl->str);
	buff_readline_update(rl);
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

int get_more_input_readline(t_rl *rl, char *prompt)
{
	int pp[2];
	int pid;

	if (pipe(pp))
		critical_error_errno_context("pipe");
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_IGN);
		close(pp[0]);
		bg_readline(pp[1], prompt);
	}
	else if (pid < 0)
		critical_error_errno_context("fork");
	else
		return (attach_input_readline(rl, pp, pid));
	ft_assert("Unreachable" == 0);
	return (0);
}

void buff_readline_update(t_rl *rl)
{
	rl->has_line = rl->cursor != rl->str.len;
}

void buff_readline_init(t_rl *ret)
{
	*ret = (t_rl){};
}

void update_context(t_rl *rl, char *ctx, char *bctx)
{
	if (!rl->should_update_ctx)
		return;
	free(ctx);
	ft_asprintf(&ctx, "%s: line %i", bctx, rl->lineno);
	if (!ctx)
		ctx = ft_strdup(bctx ? bctx : "");
}

int get_more_input_notty(t_rl *rl)
{
	char buff[4096 * 2];
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
			dyn_str_pushstr(&rl->str, "\n");
		if (ret <= 0)
			break;
		status = 0;
		dyn_str_pushnstr(&rl->str, buff, ret);
		if (ft_strnchr(buff, '\n', ret))
			break;
	}
	set_unwind_sig();
	buff_readline_update(rl);
	return (status);
}

int return_last_line(t_rl *rl, t_dyn_str *ret)
{
	int len;

	len = rl->str.len - rl->cursor;
	dyn_str_pushnstr(ret, rl->str.buff + rl->cursor, len);
	rl->cursor = 0;
	rl->str.len = 0;
	rl->has_line = false;
	if (len == 0)
		return (1);
	return (4);
}

int return_new_line(char *ctx, char *bctx, t_dyn_str *ret, t_rl *rl)
{
	char *temp;
	int len;

	rl->lineno++;
	update_context(rl, ctx, bctx);
	temp = ft_strchr(rl->str.buff + rl->cursor, '\n');
	if (temp == 0)
		return (return_last_line(rl, ret));
	len = temp - (rl->str.buff + rl->cursor) + 1;
	if (len)
		dyn_str_pushnstr(ret, rl->str.buff + rl->cursor, len);
	rl->cursor += len;
	rl->has_line = rl->cursor != rl->str.len;
	if (len == 0)
		return (1);
	return (4);
}

int buff_readline(t_rl *rl, t_dyn_str *ret, char *prompt, int input_method, char *last_cmd_status_s, t_status *res, char *ctx, char *bctx)
{
	int code;

	if (rl->has_finished)
		return (0);
	if (!rl->has_line)
	{
		if (input_method == INP_ARG || input_method == INP_FILE)
			return (rl->has_finished = true, 0);
		if (input_method == INP_STDIN_NOTTY)
			code = get_more_input_notty(rl);
		else
			code = get_more_input_readline(rl, prompt);
		if (code == 1)
			return (rl->has_finished = true, 0);
		if (code == 2)
		{
			get_g_sig()->should_unwind = SIGINT;
			set_cmd_status(*res, &(t_status){.status = CANCELED, .pid = 0, .c_c = true}, last_cmd_status_s);
			return (2);
		}
		if (input_method == INP_READLINE)
			dyn_str_push(&rl->str, '\n');
		rl->has_line = true;
	}
	return (return_new_line(ctx, bctx, ret, rl));
}
