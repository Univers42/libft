/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rl_k_vi5.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 01:18:37 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/14 01:18:39 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "x_readline.h"

void	rl_k_p_upper(t_readline *rl)
{
	short	pos;

	if ((g_rl_flags & RL_HISTORY_SEARCH_FLAG) ||
	(g_rl_flags & RL_HIGHLIGHT_FLAG))
		rl_disable_line(rl);
	if (*rl->copy_buff.buffer)
	{
		pos = rl->cord.pos;
		rl_check_str_mem(&rl->line, rl->copy_buff.buffer);
		if (!(ft_stradd(rl->line.buffer, rl->copy_buff.buffer, pos)))
			rl_err("42sh", "malloc() error", ENOMEM);
		ft_putstr(tigetstr("civis"));
		rl_print(rl->line.buffer + rl->cord.pos, &rl->cord);
		rl_go_left(rl->cord.pos - (pos +
		ft_strlen(rl->copy_buff.buffer)), &rl->cord);
		ft_putstr(tigetstr("cvvis"));
	}
}

void	rl_k_y_upper(t_readline *rl)
{
	if ((g_rl_flags & RL_HISTORY_SEARCH_FLAG) ||
	(g_rl_flags & RL_HIGHLIGHT_FLAG))
		rl_disable_line(rl);
	rl_check_str_mem(&rl->copy_buff, rl->line.buffer);
	ft_strcpy(rl->copy_buff.buffer, rl->line.buffer);
}

void	rl_k_d_upper(t_readline *rl)
{
	if ((g_rl_flags & RL_HISTORY_SEARCH_FLAG) ||
	(g_rl_flags & RL_HIGHLIGHT_FLAG))
		rl_disable_line(rl);
	rl_check_str_mem(&rl->copy_buff, rl->line.buffer + rl->cord.pos);
	ft_strcpy(rl->copy_buff.buffer, rl->line.buffer + rl->cord.pos);
	ft_str_clear(rl->line.buffer + rl->cord.pos);
	ft_putstr(tigetstr("ed"));
	rl_set_end_cord(&rl->cord);
}
