/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rl_init_temp.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 01:17:31 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/14 01:17:32 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "x_readline.h"

static t_hash_list **init_temp3(t_hash_list **table, int hash_size)
{
	char symb[2];

	symb[1] = '\0';
	symb[0] = RL_K_CTRL_X;
	table = push_hash(table, symb, rl_k_ctrl_x, hash_size);
	symb[0] = RL_K_TAB;
	table = push_hash(table, symb, rl_k_tab, hash_size);
	return (table);
}

static t_hash_list **init_temp2(t_hash_list **table, int hash_size)
{
	char symb[2];

	symb[1] = '\0';
	symb[0] = RL_K_CTRL_V;
	table = push_hash(table, symb, rl_k_ctrl_v, hash_size);
	symb[0] = RL_K_CTRL_C;
	table = push_hash(table, symb, rl_k_ctrl_c, hash_size);
	symb[0] = RL_K_ENTER;
	table = push_hash(table, symb, rl_k_enter, hash_size);
	symb[0] = RL_K_CTRL_H;
	table = push_hash(table, symb, rl_k_bcsp, hash_size);
	symb[0] = RL_K_CTRL_R;
	table = push_hash(table, symb, rl_k_ctrl_r, hash_size);
	symb[0] = RL_K_BCSP;
	table = push_hash(table, symb, rl_k_bcsp, hash_size);
	symb[0] = RL_K_CTRL_D;
	table = push_hash(table, symb, rl_k_ctrl_d, hash_size);
	symb[0] = RL_K_CTRL_J;
	table = push_hash(table, symb, rl_k_ctrl_j, hash_size);
	symb[0] = RL_K_ESC;
	table = push_hash(table, symb, rl_k_esc, hash_size);
	symb[0] = RL_K_CTRL_G;
	table = push_hash(table, symb, rl_k_ctrl_g, hash_size);
	return (init_temp3(table, hash_size));
}

t_hash_list **init_temp(int hash_size)
{
	t_hash_list **table;

	if (!(table = init_hash_table(hash_size)))
		rl_err("42sh", "malloc() error", ENOMEM);
	table = push_hash(table, tigetstr("kcub1"), rl_k_left, hash_size);
	table = push_hash(table, tigetstr("kcuf1"), rl_k_right, hash_size);
	table = push_hash(table, tigetstr("khome"), rl_k_home, hash_size);
	table = push_hash(table, tigetstr("kend"), rl_k_end, hash_size);
	table = push_hash(table, RL_K_CTRL_UP, rl_k_ctrl_up, hash_size);
	table = push_hash(table, RL_K_CTRL_DOWN, rl_k_ctrl_down, hash_size);
	table = push_hash(table, RL_K_CTRL_RIGHT, rl_k_ctrl_right, hash_size);
	table = push_hash(table, RL_K_CTRL_LEFT, rl_k_ctrl_left, hash_size);
	table = push_hash(table, tigetstr("kRIT"), rl_k_shift_right, hash_size);
	table = push_hash(table, tigetstr("kLFT"), rl_k_shift_left, hash_size);
	table = push_hash(table, tigetstr("kdch1"), rl_k_del, hash_size);
	table = push_hash(table, tigetstr("kcuu1"), rl_k_up, hash_size);
	table = push_hash(table, tigetstr("kcud1"), rl_k_down, hash_size);
	table = init_temp2(table, hash_size);
	return (table);
}
