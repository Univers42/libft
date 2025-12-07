/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hist.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 02:12:53 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/07 13:12:40 by dlesieur         ###   ########.fr       */
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

typedef struct s_status
{
	int	status;	//-1 means pending
	int	pid;	// -1 means no PID
	bool	c_c;
}t_status;

void	bg_readline(int outfd, char *prompt);
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
void	set_cmd_status(t_status res, t_status *last_cmd_status_res, char *last_cmd_status_s);
int	buff_readline(t_rl *rl, t_dyn_str *ret, char *prompt, int input_method, char *last_cmd_status_s, t_status *res, char *ctx, char *bctx);
t_dyn_str	prompt_normal(char *last_cmd_status_s, t_status *st);
t_dyn_str	parse_single_cmd(t_dyn_str hist, size_t *cur);
void	parse_history_file(t_hist *h, t_vec *env);
char	*get_hist_file_path(t_vec *ldenv);
t_vec	parse_hist_file(t_dyn_str hist);


void	buff_readline_reset(t_rl *rl)
{
	ft_memmove(rl->str.buff, rl->str.buff + rl->cursor, rl->str.len - rl->cursor);
	rl->str.len -= rl->cursor;
	if (rl->str.buff)
		rl->str.buff[rl->str.len] = 0;
	rl->cursor = 0;
	buff_readline_update(rl);
}

void	update_context(t_rl *rl, char *ctx, char *bctx)
{
    if (!rl->should_update_ctx)
        return ;
    free(ctx);
    ft_asprintf(&ctx, "%s: line %i", bctx, rl->lineno);
    if (!ctx)
        ctx = ft_strdup(bctx ? bctx : "");
}

void	buff_readline_update(t_rl *rl)
{
	rl->has_line = rl->cursor != rl->str.len;
}

void	buff_readline_init(t_rl *ret)
{
	*ret = (t_rl){};
}

void	bg_readline(int outfd, char *prompt)
{
	char	*ret;

	rl_instream = stdin;
	rl_outstream = stderr;
	ret = readline(prompt);
	if (!ret)
	{
		close(outfd);
		exit (1);
	}
	write_file(ret, outfd);
	close(outfd);
	exit(0);
}

int	attach_input_readline(t_rl *rl, int pp[2], int pid)
{
	int	status;

	close(pp[1]);
	dyn_str_append_fd(pp[0], &rl->str);
	buff_readline_update(rl);
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

int	get_more_input_readline(t_rl *rl, char *prompt)
{
	int	pp[2];
	int	pid;

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


int	return_last_line(t_rl *rl, t_dyn_str *ret)
{
	int		len;

	len = rl->str.len - rl->cursor;
	dyn_str_pushnstr(ret, rl->str.buff + rl->cursor, len);
	rl->cursor = 0;
	rl->str.len = 0;
	rl->has_line = false;
	if (len == 0)
		return (1);
	return (4);
}

int	get_more_input_notty(t_rl *rl)
{
	char	buff[4096 * 2];
	int		ret;
	int		status;

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
			break ;
		status = 0;
		dyn_str_pushnstr(&rl->str, buff, ret);
		if (ft_strnchr(buff, '\n', ret))
			break ;
	}
	set_unwind_sig();
	buff_readline_update(rl);
	return (status);
}

int	return_new_line(char *ctx, char *bctx, t_dyn_str *ret, t_rl *rl)
{
	char	*temp;
	int		len;

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

void	set_cmd_status(t_status res, t_status *last_cmd_status_res, char *last_cmd_status_s)
{
	*last_cmd_status_res = res;
	free(last_cmd_status_s);
	last_cmd_status_s = ft_itoa(res.status);
}

int	buff_readline(t_rl *rl, t_dyn_str *ret, char *prompt, int input_method, char *last_cmd_status_s, t_status *res, char *ctx, char *bctx)
{
	int		code;

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

t_dyn_str	prompt_more_input(t_vec *parse_stack)
{
	t_dyn_str	ret;
	t_token_type		curr;
	size_t		i;

	i = 0;
	dyn_str_init(&ret);
	while (i < parse_stack->len)
	{
		curr = (t_token_type)vec_idx(parse_stack, i++);
		if (curr == TOKEN_LEFT_BRACE)
			dyn_str_pushstr(&ret, "subsh");
		else if (curr == TOKEN_PIPE)
			dyn_str_pushstr(&ret, "pipe");
		else if (curr == TOKEN_LOGICAL_AND)
			dyn_str_pushstr(&ret, "cmdand");
		else if (curr == TOKEN_LOGICAL_OR)
			dyn_str_pushstr(&ret, "cmdor");
		else
			continue ;
		dyn_str_pushstr(&ret, " ");
	}
	ret.buff[ret.len - 1] = '>';
	dyn_str_pushstr(&ret, " ");
	return (ret);
}

t_dyn_str	prompt_normal(char *last_cmd_status_s, t_status *st)
{
	t_dyn_str	ret;

	dyn_str_init(&ret);
	if (st->status == 0)
		dyn_str_pushstr(&ret, GREEN_TERM);
	else
	{
		dyn_str_pushstr(&ret, RED_TERM);
		dyn_str_pushstr(&ret, last_cmd_status_s);
		dyn_str_pushstr(&ret, " ");
	}
	dyn_str_pushstr(&ret, PROMPT);
	dyn_str_pushstr(&ret, RESET_TERM);
	dyn_str_pushstr(&ret, RL_SPACER_1);
	dyn_str_pushstr(&ret, RL_SPACER_1);
	return (ret);
}

t_dyn_str	parse_single_cmd(t_dyn_str hist, size_t *cur)
{
	t_dyn_str	cmd;
	bool		bs;
	char		c;

	dyn_str_init(&cmd);
	bs = false;
	while (*cur < hist.len)
	{
		c = hist.buff[*cur];
		if (c == '\\' && !bs)
		{
			bs = true;
			(*cur)++;
			continue ;
		}
		if (c == '\n' && !bs)
		{
			(*cur)++;
			break ;
		}
		dyn_str_push(&cmd, c);
		bs = false;
		(*cur)++;
	}
	return (cmd);
}

/**
 * !? Set connfig struct , do not forget
 */
t_vec	parse_hist_file(t_dyn_str hist)
{
	size_t		cur;
	t_vec		ret;
	char		*cmd;
	t_vec_config	config;

	config = (t_vec_config){.elem_size = sizeof(char *), .initial_capacity = 32, .type_mask = VEC_TYPE_PTR};

	cur = 0;
	if (vec_init(&ret, &config) < 0)
		return (ret);
	while (cur < hist.len)
	{
		cmd = parse_single_cmd(hist, &cur).buff;
		vec_push(&ret, cmd);
		add_history(cmd);
	}
	return (ret);
}

char	*get_hist_file_path(t_vec *ldenv)
{
	t_env		*env;
	t_dyn_str	full_path;
	env = env_get(ldenv, "HOME");
	if (!env || !env->value)
	{
		warning_error("HOME is not set, can't get the history");
		return (0);
	}
	dyn_str_init(&full_path);
	dyn_str_pushstr(&full_path, env->value);
	if (!dyn_str_ends_with_str(&full_path, "/"))
		dyn_str_push(&full_path, '/');
	dyn_str_pushstr(&full_path, HIST_FILE);
	return (full_path.buff);
}

void	parse_history_file(t_hist *h, t_vec *env)
{
	t_dyn_str	hist;
	int			fd;
	char		*hist_file_path;

	hist_file_path = get_hist_file_path(env);
	if (!hist_file_path)
		return ;
	fd = open(hist_file_path, O_RDONLY, O_CREAT, 0666);
	if (fd < 0)
	{
		warning_error("Can't open the history file for reading");
		free(hist_file_path);
		return ;
	}
	dyn_str_init(&hist);
	dyn_str_append_fd(fd, &hist);
	close(fd);
	h->cmds = parse_hist_file(hist);
	h->append_fd = open(hist_file_path, O_CREAT | O_WRONLY | O_APPEND, 0666);
	if (h->append_fd < 0)
		warning_error("Can't open the history file for writing");
	free(hist_file_path);
	free(hist.buff);
}