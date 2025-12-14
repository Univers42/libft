/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rl_init_structs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 01:17:25 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/14 01:17:27 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "x_readline.h"

void		rl_init_buff(t_buff *buffer)
{
	if (!(buffer->buffer = ft_strnew(LINE_SIZE)))
		rl_err("42sh", "malloc() error", ENOMEM);
	buffer->malloc_len = LINE_SIZE;
	buffer->cur_len = 0;
}

void		rl_init_cord(t_rl_cord *cord)
{
	cord->x_cur = 0;
	cord->x_start = 0;
	cord->ws_col = 0;
	cord->ws_row = 0;
	cord->y_cur = 0;
	cord->y_start = 0;
	cord->pos = 0;
	cord->highlight_pos = 0;
	cord->x_end = 0;
	cord->y_end = 0;
}
