/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 22:51:28 by anddokhn          #+#    #+#             */
/*   Updated: 2025/12/20 21:13:38 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <readline/history.h>
#include <unistd.h>
#include "ft_readline.h"

bool worthy_of_being_remembered(t_hist *hist, t_rl *rl)
{
	// Determine actual entered command length:
	// - prefer rl->cursor (cursor points after last typed char),
	// - fallback to entire buffer length if cursor is not set.
	size_t entry_len = 0;
	if (rl->cursor > 0)
	{
		// rl->cursor is count of characters including a trailing newline pushed by xreadline
		// historically callers used rl->cursor - 1; guard against underflow
		if (rl->cursor > 0)
			entry_len = rl->cursor - 1;
	}
	if (entry_len == 0)
		entry_len = rl->str.len;

	// Only remember non-empty entries and when history is active.
	if (entry_len > 0 && hist->active &&
		( !hist->cmds.len ||
		  !str_slice_eq_str(rl->str.buff, entry_len,
							*(char **)vec_idx(&hist->cmds, hist->cmds.len - 1)) ))
	{
		return (true);
	}
	return (false);
}

void manage_history(t_hist *hist, t_rl *rl)
{
	t_repl_config *conf = get_repl_config();

	if (conf && !conf->enable_history)
		return ;
	char *hist_entry;
	char *enc_hist_entry;

	/* compute entry length the same way as worthy_of_being_remembered */
	size_t entry_len = 0;
	if (rl->cursor > 0)
	{
		/* guard: rl->cursor is count including trailing newline pushed by xreadline */
		if (rl->cursor > 0)
			entry_len = rl->cursor - 1;
	}
	if (entry_len == 0)
		entry_len = rl->str.len;

	if (entry_len == 0)
		return;

	if (worthy_of_being_remembered(hist, rl))
	{
		hist_entry = ft_strndup(rl->str.buff, entry_len);
		add_history(hist_entry);
		vec_push(&hist->cmds, &hist_entry);
		if (hist->append_fd >= 0)
		{
			enc_hist_entry = encode_cmd_hist(hist_entry).buff;
			if (write_to_file(enc_hist_entry, hist->append_fd) < 0)
			{
				warning_error("Failed to write to the history file");
				close(hist->append_fd);
				hist->append_fd = -1;
			}
			free(enc_hist_entry);
		}
	}
}

void manage_history_input(t_hist *hist, t_rl *rl, t_dyn_str *input)
{
	t_repl_config *conf = get_repl_config();
	char *hist_entry;
	char *enc_hist_entry;
	size_t entry_len;

	(void)rl;
	if (conf && !conf->enable_history)
		return ;
	if (!input || input->len == 0)
		return ;
	entry_len = input->len;
	/* trim trailing newline if present */
	if (entry_len > 0 && input->buff[entry_len - 1] == '\n')
		entry_len--;
	if (entry_len == 0)
		return ;
	/* skip duplicate of last history entry */
	if (hist->cmds.len)
	{
		char *last = *(char **)vec_idx(&hist->cmds, hist->cmds.len - 1);
		if (last && str_slice_eq_str(input->buff, entry_len, last))
			return;
	}
	hist_entry = ft_strndup(input->buff, entry_len);
	add_history(hist_entry);
	vec_push(&hist->cmds, &hist_entry);
	if (hist->append_fd >= 0)
	{
		enc_hist_entry = encode_cmd_hist(hist_entry).buff;
		if (write_to_file(enc_hist_entry, hist->append_fd) < 0)
		{
			warning_error("Failed to write to the history file");
			close(hist->append_fd);
			hist->append_fd = -1;
		}
		free(enc_hist_entry);
	}
}

void init_history(t_hist *hist, t_vec *env)
{
	*hist = (t_hist){.append_fd = -1, .active = true};
	parse_history_file(hist, env);
}

void free_hist(t_hist *hist)
{
	size_t i;

	i = 0;
	while (i < hist->cmds.len)
	{
		free(*(char **)vec_idx(&hist->cmds, i));
		i++;
	}
	free(hist->cmds.buff);
	vec_init(&hist->cmds, &(t_vec_config){0});
}
