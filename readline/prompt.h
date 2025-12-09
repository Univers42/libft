/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 17:16:55 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/09 18:49:06 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROMPT_H
# define PROMPT_H

#define _XOPEN_SOURCE 700
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <wchar.h>
#include <locale.h>
#include <errno.h>
#include <readline/readline.h>
#include "libft.h"
#include "parser.h"
#include "lexer.h"
#include "ft_colors.h"
#include "system.h"

static inline int get_cols(void)
{
	struct winsize ws;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
		return (80);
	return ((int)ws.ws_col);
}

static inline int vis_width(const char *s)
{
	mbstate_t st;
	wchar_t wc;
	size_t r;
	const char *p;
	int width;
	int w;

	if (!s)
		return (0);
	ensure_locale();
	ft_memset(&st, 0, sizeof(st));
	p = s;
	width = 0;
	while (*p)
	{
		if (*p == '\001')
		{
			p++;
			while (*p && *p != '\002')
				p++;
			if (*p == '\002')
				p++;
			continue;
		}
		r = mbrtowc(&wc, p, MB_CUR_MAX, &st);
		if (r == (size_t)-2 || r == (size_t)-1)
		{
			width++;
			p++;
			ft_memset(&st, 0, sizeof(st));
			continue;
		}
		if (r == 0)
			break;
		w = wcwidth(wc);
		if (w > 0)
			width += w;
		p += r;
	}
	return (width);
}

static inline char *shorten_path(const char *path, int max_w)
{
	t_dyn_str r;
	const char *home;
	const char *start;
	const char *last;
	const char *base;

	if (!path)
		return (ft_strdup("~"));
	home = getenv("HOME");
	start = path;
	dyn_str_init(&r);
	if (home && ft_strncmp(path, home, ft_strlen(home)) == 0)
	{
		dyn_str_push(&r, '~');
		start = path + ft_strlen(home);
	}
	last = ft_strrchr(start, '/');
	base = last ? last + 1 : start;
	if (vis_width(r.buff) + (int)ft_strlen(start) <= max_w)
	{
		dyn_str_pushstr(&r, start);
		return (r.buff);
	}
	if (vis_width(base) < max_w - 6 && *base)
	{
		dyn_str_pushstr(&r, "/...");
		if (last)
			dyn_str_pushstr(&r, last);
	}
	else
		dyn_str_pushstr(&r, start);
	return (r.buff);
}

static inline int cap_cmd(const char *cmd, t_dyn_str *out)
{
	int pp[2];
	pid_t pid;
	int st;
	char buf[256];
	ssize_t n;

	dyn_str_init(out);
	if (pipe(pp) < 0)
		return (-1);
	pid = fork();
	if (pid < 0)
	{
		close(pp[0]);
		close(pp[1]);
		return (-1);
	}
	if (pid == 0)
	{
		close(pp[0]);
		dup2(pp[1], STDOUT_FILENO);
		dup2(pp[1], STDERR_FILENO);
		close(pp[1]);
		execl("/bin/sh", "sh", "-c", cmd, (char *)NULL);
		_exit(127);
	}
	close(pp[1]);
	while ((n = read(pp[0], buf, sizeof(buf))) > 0)
		dyn_str_pushnstr(out, buf, (size_t)n);
	close(pp[0]);
	waitpid(pid, &st, 0);
	if (out->len > 0 && out->buff[out->len - 1] == '\n')
		out->buff[--out->len] = '\0';
	return (WIFEXITED(st) ? WEXITSTATUS(st) : -1);
}

static inline t_vcs_info	get_git(void)
{
	t_vcs_info	g;
	t_dyn_str	br;
	t_dyn_str	d;
	int			ret;

	g = (t_vcs_info){NULL, 0, 0};
	ret = cap_cmd("git rev-parse --abbrev-ref HEAD 2>/dev/null", &br);
	if (ret == 0 && br.len > 0)
	{
		g.ok = 1;
		g.data = br.buff;
		cap_cmd("git status --porcelain 2>/dev/null | head -1", &d);
		g.dirty = (d.len > 0);
		free(d.buff);
	}
	else
		free(br.buff);
	return (g);
}

/* ═══════════════════════════════════════════════════════════════════════════
   FORMAT TIME
   ═══════════════════════════════════════════════════════════════════════════ */

static void fmt_time(char *buf, size_t sz, long ms)
{
	if (ms < 1000)
		snprintf(buf, sz, "%ldms", ms);
	else if (ms < 60000)
		snprintf(buf, sz, "%.1fs", ms / 1000.0);
	else
		snprintf(buf, sz, "%ldm%lds", ms / 60000, (ms % 60000) / 1000);
}

# endif