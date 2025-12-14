/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rl_init_emacs_temp.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 01:17:17 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/14 01:17:18 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "x_readline.h"

static t_hash_list **init_emacs_temp2(t_hash_list **table, int hash_size)
{
	char symb[2];

	symb[1] = '\0';
	symb[0] = RL_K_CTRL_B;
	table = push_hash(table, symb, rl_k_left, hash_size);
	symb[0] = RL_K_CTRL_F;
	table = push_hash(table, symb, rl_k_right, hash_size);
	symb[0] = RL_K_CTRL_A;
	table = push_hash(table, symb, rl_k_home, hash_size);
	symb[0] = RL_K_CTRL_E;
	table = push_hash(table, symb, rl_k_end, hash_size);
	symb[0] = RL_K_CTRL_P;
	table = push_hash(table, symb, rl_k_up, hash_size);
	symb[0] = RL_K_CTRL_N;
	table = push_hash(table, symb, rl_k_down, hash_size);
	symb[0] = RL_K_CTRL_L;
	table = push_hash(table, symb, rl_k_ctrl_l, hash_size);
	symb[0] = RL_K_CTRL_U;
	table = push_hash(table, symb, rl_k_ctrl_u, hash_size);
	symb[0] = RL_K_CTRL_K;
	table = push_hash(table, symb, rl_k_ctrl_k, hash_size);
	symb[0] = RL_K_CTRL_Y;
	table = push_hash(table, symb, rl_k_ctrl_v, hash_size);
	return (table);
}

t_hash_list **init_emacs_temp(int hash_size)
{
	t_hash_list **table;
	char symb[2];

	symb[1] = '\0';
	table = init_temp(hash_size);
	table = init_emacs_temp2(table, hash_size);
	symb[0] = RL_K_CTRL_W;
	table = push_hash(table, symb, rl_k_ctrl_w, hash_size);
	symb[0] = RL_K_CTRL_T;
	table = push_hash(table, symb, rl_k_ctrl_t, hash_size);
	table = push_hash(table, RL_K_ALT_B, rl_k_ctrl_left, hash_size);
	table = push_hash(table, RL_K_ALT_F, rl_k_ctrl_right, hash_size);
	table = push_hash(table, RL_K_ALT_D, rl_k_alt_d, hash_size);
	table = push_hash(table, RL_K_ALT_R, rl_k_alt_r, hash_size);
	table = push_hash(table, RL_K_ALT_RIGHT, rl_k_alt_right, hash_size);
	table = push_hash(table, RL_K_ALT_T, rl_k_alt_t, hash_size);
	table = push_hash(table, RL_K_ALT_U, rl_k_alt_u, hash_size);
	table = push_hash(table, RL_K_ALT_L, rl_k_alt_l, hash_size);
	table = push_hash(table, RL_K_ALT_C, rl_k_alt_c, hash_size);
	table = push_hash(table, RL_K_ALT_LEFT, rl_k_alt_left, hash_size);
	return (table);
}
