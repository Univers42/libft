/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 21:09:57 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/08 02:57:15 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_readline.h"
#include "ft_vector.h"
#include "ft_string.h"
#include "ft_memory.h"
#include <fcntl.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include "trap.h"
#include "lexer.h"
#include "var.h"

t_dyn_str prompt_more_input(t_vec *parse_stack)
{
	t_dyn_str ret;
	t_token_type curr;
	size_t i;

	i = 0;
	dyn_str_init(&ret);
	while (i < parse_stack->len)
	{
		curr = (t_token_type)vec_idx(parse_stack, i++);
		if (curr == TOKEN_LEFT_BRACE)
			dyn_str_pushstr(&ret, "subsh");
		else if (curr == TOKEN_PIPE)
			dyn_str_pushstr(&ret, "pipe");
		else if (curr == TOKEN_LOGICAL_AND)
			dyn_str_pushstr(&ret, "cmdand");
		else if (curr == TOKEN_LOGICAL_OR)
			dyn_str_pushstr(&ret, "cmdor");
		else
			continue;
		dyn_str_pushstr(&ret, " ");
	}
	ret.buff[ret.len - 1] = '>';
	dyn_str_pushstr(&ret, " ");
	return (ret);
}

// ...existing code...
t_dyn_str prompt_normal(char *last_cmd_status_s, t_status *st)
{
    t_dyn_str ret;

    dyn_str_init(&ret);
    if (st->status == 0)
    {
        /* wrap non-printing ANSI with \001 and \002 for readline */
        dyn_str_pushstr(&ret, "\001");
        dyn_str_pushstr(&ret, GREEN_TERM);
        dyn_str_pushstr(&ret, "\002");
    }
    else
    {
        dyn_str_pushstr(&ret, "\001");
        dyn_str_pushstr(&ret, RED_TERM);
        dyn_str_pushstr(&ret, "\002");
        dyn_str_pushstr(&ret, last_cmd_status_s);
        dyn_str_pushstr(&ret, " ");
    }
    /* PROMPT may contain printable chars only; color reset must be wrapped */
    dyn_str_pushstr(&ret, PROMPT);
    dyn_str_pushstr(&ret, "\001");
    dyn_str_pushstr(&ret, RESET_TERM);
    dyn_str_pushstr(&ret, "\002");
    dyn_str_pushstr(&ret, RL_SPACER_1);
    dyn_str_pushstr(&ret, RL_SPACER_1);
    return (ret);
}
// ...existing code...