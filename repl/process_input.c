/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 09:39:34 by anddokhn          #+#    #+#             */
/*   Updated: 2025/12/08 23:44:48 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdbool.h>
#include "ds.h"
#include "parser.h"

bool is_empty_token_list(t_deque *tokens)
{
	t_token_type	first_token = (t_token_type)deque_idx(tokens, 0);

	if (tokens->len < 2)
		return (true);
	if (tokens->len == 2 && first_token == TOKEN_NEWLINE)
		return (true);
	return (false);
}

// void parse_input(t_state *state)
// {
// 	t_deque	tt;
// 	char	*prompt;
// 	t_parse	parser;

// 	parser = (t_parser){.res = ST_INIT};
// 	prompt = prompt_normal(&state->last_cmd_status_res, &state->last_cmd_status_s).buff;
// 	deque_tt_init(&tt, 100);
// 	get_more_tokens(&tt, &state->readline_buff,
// 					&prompt, &state->input, &state->last_cmd_status_res,
// 					&state->last_cmd_status_s, &state->input_method,
// 					&state->context, &state->base_context,
// 					(int *)&state->should_exit);
// 	if (g_should_unwind)
// 		set_cmd_status(&state->last_cmd_status_res, (t_status){.status = CANCELED, .c_c = true}, &state->last_cmd_status_s);
// 	free(parser.parse_stack.buff);
// 	parser.parse_stack = (t_vec){};
// 	free(tt.buff);
// 	state->should_exit |= (g_should_unwind && state->input_method != INP_READLINE) || state->readline_buff.has_finished;
// }
