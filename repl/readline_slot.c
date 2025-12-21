/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_slot.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 20:14:42 by marvin            #+#    #+#             */
/*   Updated: 2025/12/21 20:14:42 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdbool.h>
#include "ft_wctype.h" 
#include "ft_readline.h" 
#include <stdint.h>
#include <unistd.h>

int	readline_cmd(t_rl *rl, t_getmore_ctx *ctx)
{
	int				stat;
	t_xreadline_ctx	xctx;

	xctx.ret = ctx->input;
	xctx.prompt = *ctx->prompt;
	xctx.last_cmd_status_res = ctx->last_cmd_status_res;
	xctx.last_cmd_status_s = ctx->last_cmd_status_s;
	xctx.input_method = ctx->input_method;
	xctx.context = ctx->context;
	xctx.base_context = ctx->base_context;
	stat = xreadline(rl, &xctx);
	if (stat == 0)
		return (1);
	if (stat == 2)
	{
		if (*ctx->input_method != INP_READLINE)
			*ctx->should_exit = true;
		return (2);
	}
	return (0);
}

bool	ends_with_bs_nl(t_dyn_str s)
{
	size_t	i;
	bool	unterminated;

	if (s.len == 0)
		return (false);
	i = s.len;
	unterminated = false;
	if (s.buff[--i] != '\n')
		return (false);
	while (i > 0)
	{
		i--;
		if (s.buff[i] == '\\')
			unterminated = !unterminated;
		else
			break ;
	}
	return (unterminated);
}

void	extend_bs(t_rl *rl, t_getmore_ctx *ctx)
{
	char	*tmp_prompt;
	char	*old_prompt;

	while (ends_with_bs_nl(*ctx->input))
	{
		dyn_str_pop(ctx->input);
		dyn_str_pop(ctx->input);
		tmp_prompt = ft_strdup("> ");
		if (!tmp_prompt)
			return ;
		old_prompt = *ctx->prompt;
		*ctx->prompt = tmp_prompt;
		if (readline_cmd(rl, ctx))
		{
			*ctx->prompt = old_prompt;
			free(tmp_prompt);
			return ;
		}
		*ctx->prompt = old_prompt;
		free(tmp_prompt);
	}
}

/* sanitize input buffer: replace invalid UTF-8 leading/continuation with '?' so
   downstream mbrtowc/vis_width/tokenizer do not loop on invalid bytes */
void	sanitize_input_utf8(t_dyn_str *input)
{
	mbstate_t	st;
	wchar_t		wc;
	size_t		r;
	size_t		i;

	i = 0;
	ft_memset(&st, 0, sizeof(st));
	while (i < input->len)
	{
		r = ft_mbrtowc(&wc, input->buff + i, (size_t)(input->len - i), &st);
		if (r == (size_t) - 1)
		{
			input->buff[i] = '?';
			ft_memset(&st, 0, sizeof(st));
			i++;
			continue ;
		}
		if (r == (size_t) - 2)
			break ;
		if (r == 0)
			break ;
		i += r;
	}
}