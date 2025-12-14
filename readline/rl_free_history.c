/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rl_free_history.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 01:17:13 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/14 02:28:57 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "x_readline.h"

void rl_free_history(t_rl_history *history)
{
	int fd = -1;
	int i;
	char *histfile = NULL;
	char *hf_env;
	char *home;
	char *tmp_alloc = NULL;

	/* Prefer explicit HISTFILE (VARS); otherwise try $HOME/.42sh_history,
	   then fallback to local "./.42sh_history". Do not abort the program if
	   none of these are writable — just warn and skip saving. */
	hf_env = get_var("HISTFILE", VARS);
	home = get_var("HOME", ENV);

	rl_get_hist_size(history);

	/* Try HISTFILE from vars/env first (do not free it) */
	if (hf_env)
	{
		histfile = hf_env;
		fd = open(histfile, O_WRONLY | O_TRUNC | O_CREAT,
				  S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	}

	/* Next try $HOME + RL_HISTORY_FILE */
	if (fd == -1 && home)
	{
		tmp_alloc = ft_strjoin(home, RL_HISTORY_FILE);
		if (tmp_alloc)
			fd = open(tmp_alloc, O_WRONLY | O_TRUNC | O_CREAT,
					  S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	}

	/* Final fallback: local file in current working dir */
	if (fd == -1)
	{
		if (!tmp_alloc)
			tmp_alloc = ft_strdup("./.42sh_history");
		if (tmp_alloc)
			fd = open(tmp_alloc, O_WRONLY | O_TRUNC | O_CREAT,
					  S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	}

	/* If still unable to open, warn and skip saving history (do not call rl_err). */
	if (fd == -1)
	{
		ft_putendl_fd("42sh: warning: cannot open histfile for writing, skipping history save", STDERR_FILENO);
		if (tmp_alloc)
			ft_strdel(&tmp_alloc);
		ft_free_dar(history->history_buff);
		return;
	}

	i = 0;
	if (history->histfilesize < history->hist_len)
		i = history->hist_len - history->histfilesize;
	while (i < history->hist_len && history->history_buff[i])
		ft_putendl_fd(history->history_buff[i++], fd);
	if (close(fd) == -1)
		ft_putendl_fd("42sh: warning: close histfile error", STDERR_FILENO);
	if (tmp_alloc)
		ft_strdel(&tmp_alloc);
	ft_free_dar(history->history_buff);
}
