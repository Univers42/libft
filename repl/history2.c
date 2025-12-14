/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 22:51:28 by anddokhn          #+#    #+#             */
/*   Updated: 2025/12/14 01:46:42 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_readline.h"
#include <readline/history.h>
#include <unistd.h>

bool worthy_of_being_remembered(t_hist *hist, t_rl *rl)
{

	if (rl->cursor > 1 && hist->active && (!hist->cmds.len || !str_slice_eq_str(rl->str.buff, rl->cursor - 1, *(char **)vec_idx(&hist->cmds, hist->cmds.len - 1))))
	{
		return (true);
	}
	return (false);
}

void manage_history(t_hist *hist, t_rl *rl)
{
	char *hist_entry;
	char *enc_hist_entry;

	if (worthy_of_being_remembered(hist, rl))
	{
		hist_entry = ft_strndup(rl->str.buff, rl->cursor - 1);
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
