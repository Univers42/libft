/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 07:35:12 by anddokhn          #+#    #+#             */
/*   Updated: 2025/12/20 20:54:02 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prompt.h"
#include "ft_readline.h" /* access get_repl_config() */


/* ═══════════════════════════════════════════════════════════════════════════
   BUILD PROMPT
   ═══════════════════════════════════════════════════════════════════════════ */

t_dyn_str prompt_normal(t_status *st_res, char **st_s)
{
	t_dyn_str p;
	t_dyn_str cwd;
	char *user;
	char *short_path;
	char time_buf[32];
	int cols;
	int status;
	int line_w;
	int chrono_w;
	int pad;

	static char *cached_git_branch = NULL;
	static int cached_git_dirty = 0;
	static char *cached_cwd = NULL;
	char *curr_cwd;
	int need_refresh = 0;
	t_repl_config *conf = get_repl_config();

	(void)st_s;
	dyn_str_init(&p);
	ensure_locale();
	user = getenv("USER");
	if (!user)
		user = "inferno";
	cap_cmd("pwd", &cwd);
	curr_cwd = cwd.buff;

	// Only refresh git info if cwd changed AND vcs enabled
	if ((conf == NULL || conf->enable_vcs) && (cached_cwd == NULL || strcmp(cached_cwd, curr_cwd) != 0))
		need_refresh = 1;
	if (need_refresh && (conf == NULL || conf->enable_vcs))
	{
		t_vcs_info git = get_git();
		if (cached_git_branch)
			free(cached_git_branch);
		cached_git_branch = git.data ? ft_strdup(git.data) : NULL;
		cached_git_dirty = git.dirty;
		if (cached_cwd)
			free(cached_cwd);
		cached_cwd = ft_strdup(curr_cwd);
		if (git.data)
			free(git.data);
	}
	cols = get_cols();
	status = st_res->status;
	short_path = shorten_path(cwd.buff, cols - 60);

	/* ══════════════════════════════════════════════════════════════════════
	   LINE 1: ╭─  user ═══  path ║  branch ●
	   ══════════════════════════════════════════════════════════════════════ */

	/* Opening decoration */
	dyn_str_pushstr(&p, FG_CHARCOAL);
	dyn_str_pushstr(&p, "╭─");
	dyn_str_pushstr(&p, RESET);

	/* User segment */
	dyn_str_pushstr(&p, " ");
	dyn_str_pushstr(&p, FG_MOLTEN);
	dyn_str_pushstr(&p, USER_ICON);
	dyn_str_pushstr(&p, " ");
	dyn_str_pushstr(&p, BOLD);
	dyn_str_pushstr(&p, user);
	dyn_str_pushstr(&p, RESET);

	/* Separator */
	dyn_str_pushstr(&p, FG_CHARCOAL);
	dyn_str_pushstr(&p, SEP_L);
	dyn_str_pushstr(&p, RESET);

	/* Path segment */
	dyn_str_pushstr(&p, FG_EMBER);
	dyn_str_pushstr(&p, DIR_ICON);
	dyn_str_pushstr(&p, " ");
	dyn_str_pushstr(&p, short_path);
	dyn_str_pushstr(&p, RESET);

	/* Git segment */
	if (cached_git_branch)
	{
		dyn_str_pushstr(&p, FG_CHARCOAL);
		dyn_str_pushstr(&p, SEP_GIT);
		dyn_str_pushstr(&p, RESET);
		if (cached_git_dirty)
			dyn_str_pushstr(&p, FG_PURPLE);
		else
			dyn_str_pushstr(&p, FG_ASH);
		dyn_str_pushstr(&p, GIT_ICON);
		dyn_str_pushstr(&p, " ");
		dyn_str_pushstr(&p, cached_git_branch);
		dyn_str_pushstr(&p, RESET);
	}

	/* Status code if error */
	if (status != 0)
	{
		dyn_str_pushstr(&p, " ");
		dyn_str_pushstr(&p, FG_FIRE);
		dyn_str_pushstr(&p, BOLD);
		dyn_str_pushstr(&p, "⟦");
		dyn_str_pushstr(&p, RESET);
		dyn_str_pushstr(&p, FG_WARN);
		char stat_buf[16];
		snprintf(stat_buf, sizeof(stat_buf), "%d", status);
		dyn_str_pushstr(&p, stat_buf);
		dyn_str_pushstr(&p, RESET);
		dyn_str_pushstr(&p, FG_FIRE);
		dyn_str_pushstr(&p, BOLD);
		dyn_str_pushstr(&p, "⟧");
		dyn_str_pushstr(&p, RESET);
	}

	/* ══════════════════════════════════════════════════════════════════════
	   RIGHT-ALIGNED CHRONO (only if enabled)
	   ══════════════════════════════════════════════════════════════════════ */
	if (conf == NULL || conf->enable_chrono)
	{
		fmt_time(time_buf, sizeof(time_buf), get_chrono()->last_ms);
		line_w = vis_width(p.buff);
		chrono_w = 3 + (int)ft_strlen(time_buf); /* "⏱ " + time */
		pad = cols - line_w - chrono_w - 1;
		if (pad < 2)
			pad = 2;
		while (pad-- > 0)
			dyn_str_push(&p, ' ');
		if (get_chrono()->last_ms >= 100)
			dyn_str_pushstr(&p, FG_WARN);
		else
			dyn_str_pushstr(&p, FG_DIM);
		dyn_str_pushstr(&p, TIMER_ICON);
		dyn_str_pushstr(&p, " ");
		dyn_str_pushstr(&p, time_buf);
		dyn_str_pushstr(&p, RESET);
	}
	// else: skip chrono entirely (no padding added)

	/* ══════════════════════════════════════════════════════════════════════
	   LINE 2: status circle (color by ultimate state)
	   ══════════════════════════════════════════════════════════════════════ */
	dyn_str_pushstr(&p, "\n");
	dyn_str_pushstr(&p, FG_CHARCOAL);
	dyn_str_pushstr(&p, "╰─");
	dyn_str_pushstr(&p, RESET);

	// Status color logic - priority: signal > exit_code > success
	if (st_res->c_c)
	{
		dyn_str_pushstr(&p, FG_PURPLE); // signal (Ctrl+C, etc.)
		dyn_str_pushstr(&p, BOLD);
		dyn_str_pushstr(&p, " ●");
	}
	else if (status == 0)
	{
		dyn_str_pushstr(&p, FG_SUCCESS); // green = success
		dyn_str_pushstr(&p, BOLD);
		dyn_str_pushstr(&p, " ●");
	}
	else if (status == 1)
	{
		dyn_str_pushstr(&p, FG_BLUE); // blue = generic error
		dyn_str_pushstr(&p, BOLD);
		dyn_str_pushstr(&p, " ●");
	}
	else if (status >= 128)
	{
		dyn_str_pushstr(&p, FG_WARN); // yellow = signal termination
		dyn_str_pushstr(&p, BOLD);
		dyn_str_pushstr(&p, " ●");
	}
	else
	{
		dyn_str_pushstr(&p, FG_FIRE); // red = other error
		dyn_str_pushstr(&p, BOLD);
		dyn_str_pushstr(&p, " ●");
	}
	dyn_str_pushstr(&p, RESET);
	dyn_str_pushstr(&p, " ");

	free(cwd.buff);
	free(short_path);
	return (p);
}

/* ═══════════════════════════════════════════════════════════════════════════
   MORE INPUT PROMPT
   ═══════════════════════════════════════════════════════════════════════════ */

t_dyn_str prompt_more_input(t_parse *parser)
{
	t_dyn_str ret;
	t_token_type curr;
	size_t i;

	dyn_str_init(&ret);
	dyn_str_pushstr(&ret, FG_CHARCOAL);
	i = 0;
	while (i < parser->stack.len)
	{
		curr = (t_token_type)(uintptr_t)vec_idx(&parser->stack, i++);
		if (curr == TOKEN_LEFT_BRACE)
			dyn_str_pushstr(&ret, "subsh");
		else if (curr == TOKEN_PIPE)
			dyn_str_pushstr(&ret, "pipe");
		else if (curr == TOKEN_LOGICAL_AND)
			dyn_str_pushstr(&ret, "and");
		else if (curr == TOKEN_LOGICAL_OR)
			dyn_str_pushstr(&ret, "or");
		else
			continue;
		dyn_str_pushstr(&ret, " ");
	}
	dyn_str_pushstr(&ret, RESET);
	dyn_str_pushstr(&ret, FG_EMBER);
	dyn_str_pushstr(&ret, "..> ");
	dyn_str_pushstr(&ret, RESET);
	return (ret);
}
