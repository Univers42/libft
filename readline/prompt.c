/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 07:35:12 by anddokhn          #+#    #+#             */
/*   Updated: 2025/12/09 15:00:28 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>
#include "libft.h"
#include "parser.h"
#include "lexer.h"
#include <sys/wait.h>

t_dyn_str prompt_more_input(t_parse *parser)
{
	t_dyn_str ret;
	t_token_type curr;
	size_t i;

	i = 0;
	dyn_str_init(&ret);
	while (i < parser->stack.len)
	{
		curr = (t_token_type)vec_idx(&parser->stack, i++);
		if (curr == TOKEN_LEFT_BRACE)
			dyn_str_pushstr(&ret, "subsh");
		else if (curr == TOKEN_PIPE)
			dyn_str_pushstr(&ret, "pipe");
		else if (curr == TOKEN_LOGICAL_AND)
			dyn_str_pushstr(&ret, "cmdand");
		else if (curr == TOKEN_LOGICAL_OR)
			dyn_str_pushstr(&ret, "cmdor");
		else
			continue;
		dyn_str_pushstr(&ret, " ");
	}
	ret.buff[ret.len - 1] = '>';
	dyn_str_pushstr(&ret, " ");
	return (ret);
}

/**
 * @brief Capture stdout of a shell command using pipe redirection
 * @param cmd Shell command to execute (via sh -c)
 * @param out Output buffer (caller must free out.buff)
 * @return 0 on success, -1 on error
 */
static int capture_output(const char *cmd, t_dyn_str *out)
{
	int pipefd[2];
	pid_t pid;
	int status;
	char buf[512];
	ssize_t n;

	dyn_str_init(out);
	if (pipe(pipefd) < 0)
		return (-1);
	pid = fork();
	if (pid < 0)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	if (pid == 0)
	{
		/* child: redirect stdout to pipe write end */
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		execl("/bin/sh", "sh", "-c", cmd, (char *)NULL);
		exit(127);
	}
	/* parent: read from pipe */
	close(pipefd[1]);
	while ((n = read(pipefd[0], buf, sizeof(buf))) > 0)
		dyn_str_pushnstr(out, buf, (size_t)n);
	close(pipefd[0]);
	waitpid(pid, &status, 0);
	/* trim trailing newline if present */
	if (out->len > 0 && out->buff[out->len - 1] == '\n')
		out->buff[--out->len] = '\0';
	return (0);
}

/**
 * @brief Get current git branch name (empty string if not in repo)
 */
static t_dyn_str get_git_branch(void)
{
	t_dyn_str branch;

	if (capture_output("git rev-parse --abbrev-ref HEAD 2>/dev/null", &branch) < 0)
		dyn_str_init(&branch);
	return (branch);
}

/**
 * @brief Build a beautiful, dynamic prompt with git info
 * Format: [user@host:dir (branch)] status ❯
 */
t_dyn_str prompt_normal(t_status *last_cmd_status_res, char **last_cmd_status_s)
{
	t_dyn_str ret;
	t_dyn_str git_branch;
	t_dyn_str cwd;
	char *user;
	char *short_cwd;

	dyn_str_init(&ret);

	/* Get user and short cwd */
	user = getenv("USER");
	if (!user)
		user = "user";
	if (capture_output("pwd | sed \"s|$HOME|~|\"", &cwd) < 0)
		dyn_str_init(&cwd);
	short_cwd = cwd.buff ? cwd.buff : "~";

	/* Get git branch if in repo */
	git_branch = get_git_branch();

	/* Build prompt with proper readline escapes:
	   \001 = RL_PROMPT_START_IGNORE
	   \002 = RL_PROMPT_END_IGNORE */

	/* Opening bracket */
	dyn_str_pushstr(&ret, "\001");
	dyn_str_pushstr(&ret, ANSI_RESET);
	dyn_str_pushstr(&ret, "\002[");

	/* user@host:dir */
	dyn_str_pushstr(&ret, "\001\033[36m\002"); /* cyan */
	dyn_str_pushstr(&ret, user);
	dyn_str_pushstr(&ret, "\001\033[0m\002:");
	dyn_str_pushstr(&ret, "\001\033[34m\002"); /* blue */
	dyn_str_pushstr(&ret, short_cwd);
	dyn_str_pushstr(&ret, "\001\033[0m\002");

	/* Git branch if present */
	if (git_branch.len > 0)
	{
		dyn_str_pushstr(&ret, " \001\033[35m\002("); /* magenta */
		dyn_str_pushnstr(&ret, git_branch.buff, git_branch.len);
		dyn_str_pushstr(&ret, ")\001\033[0m\002");
	}

	/* Closing bracket */
	dyn_str_pushstr(&ret, "]");

	/* Status code (if non-zero, show in red) */
	if (last_cmd_status_res->status != 0)
	{
		dyn_str_pushstr(&ret, " ");
		dyn_str_pushstr(&ret, ANSI_RED);
		dyn_str_pushstr(&ret, *last_cmd_status_s);
		dyn_str_pushstr(&ret, ANSI_RESET);
	}

	/* Arrow prompt */
	dyn_str_pushstr(&ret, " ");
	if (last_cmd_status_res->status == 0)
		dyn_str_pushstr(&ret, ANSI_GREEN);
	else
		dyn_str_pushstr(&ret, ANSI_RED);
	dyn_str_pushstr(&ret, "❯ ");
	dyn_str_pushstr(&ret, ANSI_RESET);

	/* Add spacers (invisible to readline) to prevent drift */
	dyn_str_pushstr(&ret, RL_SPACER_1);
	dyn_str_pushstr(&ret, RL_SPACER_1);

	/* Cleanup */
	free(cwd.buff);
	free(git_branch.buff);

	return (ret);
}
