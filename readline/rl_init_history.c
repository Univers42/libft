/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rl_init_history.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 01:17:21 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/14 01:17:22 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "x_readline.h"

static size_t rl_find_hist_len(char *path)
{
	int fd;
	char *line;
	size_t len;

	if ((fd = open(path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR)) == -1)
		rl_err("42sh", "open() error", NOERROR);
	len = 0;
	/* Use new get_next_line(int) prototype that returns a malloc'd line or NULL */
	while ((line = get_next_line(fd)))
	{
		ft_strdel(&line);
		++len;
	}
	if (close(fd) == -1)
		rl_err("42sh", "close() error", NOERROR);
	return (len);
}

static void rl_set_hist_buff(char *path, t_rl_history *history,
							 int hist_len)
{
	int fd;
	char *line;
	int skip;

	if (!(history->history_buff = ft_darnew(history->histsize)))
		rl_err("42sh", "malloc() error", ENOMEM);
	if ((fd = open(path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR)) == -1)
		rl_err("42sh", "open() error", NOERROR);
	skip = 0;
	if (hist_len > history->histsize)
		skip = hist_len - history->histsize;
	/* Skip first 'skip' lines */
	while (skip > 0 && (line = get_next_line(fd)))
	{
		ft_strdel(&line);
		--skip;
	}
	/* Read up to history->histsize lines into history_buff */
	while (history->hist_len != history->histsize && (line = get_next_line(fd)))
	{
		history->history_buff[history->hist_len++] = line;
	}
	if (close(fd) == -1)
		rl_err("42sh", "close() error", NOERROR);
	history->hist_index = history->hist_len;
	history->cur_command_nb = 0;
}

void rl_init_history(t_rl_history *history)
{
	char *path;
	char *rl_histsize;
	char *rl_histfilesize;

	if ((get_var("HOME", ENV)))
	{
		if (!(path = ft_strjoin(get_var("HOME", ENV), RL_HISTORY_FILE)))
			rl_err("42sh", "malloc() error", ENOMEM);
	}
	else if (!(path = ft_strdup(RL_HISTORY_FILE)))
		rl_err("42sh", "malloc() error", ENOMEM);
	set_var("HISTFILE", path, VARS);
	if (!(rl_histsize = get_var("HISTSIZE", VARS)))
	{
		rl_histsize = RL_HISTSIZE;
		set_var("HISTSIZE", rl_histsize, VARS);
	}
	if (!(rl_histfilesize = get_var("HISTFILESIZE", VARS)))
	{
		rl_histfilesize = RL_HISTFILESIZE;
		set_var("HISTFILESIZE", rl_histfilesize, VARS);
	}
	rl_get_hist_size(history);
	rl_set_hist_buff(path, history, rl_find_hist_len(path));
	ft_strdel(&path);
}
