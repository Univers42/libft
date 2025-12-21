/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffered_readline_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 23:27:10 by marvin            #+#    #+#             */
/*   Updated: 2025/12/20 23:27:12 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"
#include <errno.h>
#include <unistd.h>
#include "ft_stdio.h"
#include "trap.h"
#include <stdio.h> 

void buff_readline_update(t_rl *l)
{
	l->has_line = l->cursor != l->str.len;
}

void buff_readline_reset(t_rl *l)
{
	ft_memmove(l->str.buff, l->str.buff + l->cursor, l->str.len - l->cursor);
	l->str.len -= l->cursor;
	if (l->str.buff)
		l->str.buff[l->str.len] = 0;
	l->cursor = 0;
	buff_readline_update(l);
}

void buff_readline_init(t_rl *ret)
{
	*ret = (t_rl){0};
}

void update_context(t_rl *rl, char **context, char **base_context)
{
	char *new_ctx;
	int len;

	if (!rl->should_update_ctx)
		return;
	len = ft_strlen(*base_context) + 16;
	new_ctx = (char *)malloc((size_t)len);
	if (!new_ctx)
		return;
	snprintf(new_ctx, (size_t)len, "%s: line %d", *base_context, rl->lineno);
	if (*context)
		free(*context);
	*context = new_ctx;
}

int get_more_input_notty(t_rl *rl)
{
	char buff[4096 * 2];
	int ret;
	int status;

	status = 1;
	
	{
		t_repl_config *rc = get_repl_config();
		if (rc == NULL || rc->handle_signals)
			set_unwind_sig_norestart();
	}
	while (1)
	{
		ret = read(0, buff, sizeof(buff));
		if (ret < 0 && errno == EINTR)
			status = 2;
		if (ret == 0)
			rl->has_finished = true;
		if (ret == 0)
			dyn_str_pushstr(&rl->str, "\n");
		if (ret <= 0)
			break;
		status = 0;
		dyn_str_pushnstr(&rl->str, buff, ret);
		if (ft_strnchr(buff, '\n', ret))
			break;
	}
	
	{
		t_repl_config *rc = get_repl_config();
		if (rc == NULL || rc->handle_signals)
			set_unwind_sig();
	}
	buff_readline_update(rl);
	return (status);
}
