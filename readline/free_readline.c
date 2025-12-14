/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_readline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 01:16:20 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/14 01:16:21 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "x_readline.h"

void		rl_clr_data(t_readline *rl)
{
	rl_clr_buff(&rl->line);
	rl_clr_buff(&rl->save_line);
	rl_clr_buff(&rl->history.save_line);
	rl_clr_buff(&rl->history.search);
	rl_init_cord(&rl->cord);
	g_rl_flags = RL_INIT_FLAGS;
}

void		rl_clr_buff(t_buff *buff)
{
	ft_str_clear(buff->buffer);
	buff->cur_len = 0;
}
