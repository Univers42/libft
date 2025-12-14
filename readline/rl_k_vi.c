/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rl_k_vi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 01:18:19 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/14 01:18:20 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "x_readline.h"

void	rl_k_i_lower(t_readline *rl)
{
	if ((g_rl_flags & RL_HISTORY_SEARCH_FLAG) ||
	(g_rl_flags & RL_HIGHLIGHT_FLAG))
		rl_disable_line(rl);
	g_rl_flags |= RL_VI_INPUT_MODE;
	g_rl_flags &= ~RL_VI_COMMAND_MODE;
}

void	rl_k_i_upper(t_readline *rl)
{
	g_rl_flags |= RL_VI_INPUT_MODE;
	g_rl_flags &= ~RL_VI_COMMAND_MODE;
	rl_k_home(rl);
}

void	rl_k_a_lower(t_readline *rl)
{
	g_rl_flags |= RL_VI_INPUT_MODE;
	g_rl_flags &= ~RL_VI_COMMAND_MODE;
	rl_k_right(rl);
}

void	rl_k_a_upper(t_readline *rl)
{
	g_rl_flags |= RL_VI_INPUT_MODE;
	g_rl_flags &= ~RL_VI_COMMAND_MODE;
	rl_k_end(rl);
}

void	rl_k_sharp(t_readline *rl)
{
	if (!(ft_stradd(rl->line.buffer, RL_K_SHARP, 0)))
		rl_err("42sh", "malloc() error", ENOMEM);
	rl_disable_line(rl);
	rl_k_enter(rl);
}
