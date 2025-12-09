/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xreadline.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 07:23:53 by anddokhn          #+#    #+#             */
/*   Updated: 2025/12/09 00:10:44 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <readline/readline.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <unistd.h>

uint32_t g_should_unwind = 0;

static int return_last_line(t_rl *rl, t_dyn_str *ret)
{
	int len;

	len = rl->str.len - rl->cursor;
	dyn_str_pushnstr(ret, rl->str.buff + rl->cursor, len);
	rl->cursor = 0;
	rl->str.len = 0;
	rl->has_line = false;
	if (len == 0)
		return (1);
	return (4);
}

static int return_new_line(t_rl *rl, t_dyn_str *ret, char **context, char **base_context)
{
	char *temp;
	int len;

	rl->lineno++;
	update_context(rl, context, base_context);
	temp = ft_strchr(rl->str.buff + rl->cursor, '\n');
	if (temp == 0)
		return (return_last_line(rl, ret));
	len = temp - (rl->str.buff + rl->cursor) + 1;
	if (len)
		dyn_str_pushnstr(ret, rl->str.buff + rl->cursor, len);
	rl->cursor += len;
	rl->has_line = rl->cursor != rl->str.len;
	if (len == 0)
		return (1);
	return (4);
}

int xreadline(t_rl *rl, t_dyn_str *ret, char *prompt, t_status *last_cmd_status_res, char **last_cmd_status_s, int *input_method, char **context, char **base_context)
{
	int code;

	if (rl->has_finished)
		return (0);
	if (!rl->has_line)
	{
		if (*input_method == INP_ARG || *input_method == INP_FILE)
			return (rl->has_finished = true, 0);
		if (*input_method == INP_STDIN_NOTTY)
			code = get_more_input_notty(rl);
		else
			code = get_more_input_readline(rl, prompt);
		if (code == 1)
			return (rl->has_finished = true, 0);
		if (code == 2)
		{
			g_should_unwind = SIGINT;
			set_cmd_status(last_cmd_status_res, (t_status){.status = CANCELED, .c_c = true}, last_cmd_status_s);
			return (2);
		}
		if (*input_method == INP_READLINE)
			dyn_str_push(&rl->str, '\n');
		rl->has_line = true;
	}
	return (return_new_line(rl, ret, context, base_context));
}
