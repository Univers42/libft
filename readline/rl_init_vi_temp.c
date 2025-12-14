/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rl_init_vi_temp.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 01:17:35 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/14 01:17:37 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "x_readline.h"

static t_hash_list **init_vi_temp2(t_hash_list **table, int hash_size)
{
	table = push_hash(table, RL_K_X_UPPER, rl_k_x_upper, hash_size);
	table = push_hash(table, RL_K_X_LOWER, rl_k_x_lower, hash_size);
	table = push_hash(table, RL_K_V, rl_k_v, hash_size);
	table = push_hash(table, RL_K_F_LOWER, rl_k_f_lower, hash_size);
	table = push_hash(table, RL_K_F_UPPER, rl_k_f_upper, hash_size);
	table = push_hash(table, RL_K_SEMICOLON, rl_k_semicolon, hash_size);
	table = push_hash(table, RL_K_COMMA, rl_k_comma, hash_size);
	table = push_hash(table, RL_K_R_LOWER, rl_k_r_lower, hash_size);
	table = push_hash(table, RL_K_R_UPPER, rl_k_r_upper, hash_size);
	table = push_hash(table, RL_K_P_LOWER, rl_k_p_lower, hash_size);
	table = push_hash(table, RL_K_P_UPPER, rl_k_ctrl_v, hash_size);
	table = push_hash(table, RL_K_Y_UPPER, rl_k_y_upper, hash_size);
	table = push_hash(table, RL_K_D_UPPER, rl_k_d_upper, hash_size);
	table = push_hash(table, RL_K_C_UPPER, rl_k_d_upper, hash_size);
	return (table);
}

t_hash_list **init_vi_temp(int hash_size)
{
	t_hash_list **table;

	table = init_temp(hash_size);
	table = push_hash(table, RL_K_I_LOWER, rl_k_i_lower, hash_size);
	table = push_hash(table, RL_K_I_UPPER, rl_k_i_upper, hash_size);
	table = push_hash(table, RL_K_A_LOWER, rl_k_a_lower, hash_size);
	table = push_hash(table, RL_K_A_UPPER, rl_k_a_upper, hash_size);
	table = push_hash(table, RL_K_SHARP, rl_k_sharp, hash_size);
	table = push_hash(table, RL_K_J, rl_k_down, hash_size);
	table = push_hash(table, RL_K_K, rl_k_up, hash_size);
	table = push_hash(table, RL_K_L, rl_k_left, hash_size);
	table = push_hash(table, RL_K_H, rl_k_right, hash_size);
	table = push_hash(table, RL_K_W_UPPER, rl_k_ctrl_right, hash_size);
	table = push_hash(table, RL_K_W_LOWER, rl_k_ctrl_right, hash_size);
	table = push_hash(table, RL_K_B_UPPER, rl_k_ctrl_left, hash_size);
	table = push_hash(table, RL_K_B_LOWER, rl_k_ctrl_left, hash_size);
	table = push_hash(table, RL_K_ZERO, rl_k_home, hash_size);
	table = push_hash(table, RL_K_DOLLAR, rl_k_end, hash_size);
	table = push_hash(table, RL_K_SPACE, rl_k_right, hash_size);
	table = push_hash(table, RL_K_E_UPPER, rl_k_e, hash_size);
	table = push_hash(table, RL_K_E_LOWER, rl_k_e, hash_size);
	table = push_hash(table, RL_K_CARAT, rl_k_carat, hash_size);
	table = push_hash(table, RL_K_S_UPPER, rl_k_s_upper, hash_size);
	table = push_hash(table, RL_K_S_LOWER, rl_k_s_lower, hash_size);
	return (init_vi_temp2(table, hash_size));
}
