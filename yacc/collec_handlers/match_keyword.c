/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   match_keyword.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 00:49:25 by marvin            #+#    #+#             */
/*   Updated: 2025/12/20 00:49:25 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_yacc.h"

/* helper: match an exact keyword and ensure it's not part of a longer identifier */
static bool	match_exact_kw(t_dyn_str *input, const char *kw)
{
	size_t	pos = input->pos;
	size_t	len;

	ft_assert(input && input->buff && kw);
	if (pos >= input->len)
		return (false);
	len = ft_strlen(kw);
	if (pos + len <= input->len &&
		ft_strncmp(input->buff + pos, kw, len) == 0 &&
		(pos + len >= input->len ||
		 (!ft_isalnum(dyn_str_idx(input, pos + len)) &&
		  dyn_str_idx(input, pos + len) != '_')))
	{
		input->pos = pos + len;
		return (true);
	}
	return (false);
}

/* per-keyword matchers used by yacc rules (order-independent; registration order matters) */
bool	match_kw_if(t_dyn_str *input)       { return match_exact_kw(input, "if"); }
bool	match_kw_then(t_dyn_str *input)     { return match_exact_kw(input, "then"); }
bool	match_kw_else(t_dyn_str *input)     { return match_exact_kw(input, "else"); }
bool	match_kw_elif(t_dyn_str *input)     { return match_exact_kw(input, "elif"); }
bool	match_kw_fi(t_dyn_str *input)       { return match_exact_kw(input, "fi"); }
bool	match_kw_for(t_dyn_str *input)      { return match_exact_kw(input, "for"); }
bool	match_kw_while(t_dyn_str *input)    { return match_exact_kw(input, "while"); }
bool	match_kw_do(t_dyn_str *input)       { return match_exact_kw(input, "do"); }
bool	match_kw_done(t_dyn_str *input)     { return match_exact_kw(input, "done"); }
bool	match_kw_case(t_dyn_str *input)     { return match_exact_kw(input, "case"); }
bool	match_kw_esac(t_dyn_str *input)     { return match_exact_kw(input, "esac"); }
bool	match_kw_in(t_dyn_str *input)       { return match_exact_kw(input, "in"); }
bool	match_kw_function(t_dyn_str *input) { return match_exact_kw(input, "function"); }
bool	match_kw_select(t_dyn_str *input)   { return match_exact_kw(input, "select"); }
bool	match_kw_return(t_dyn_str *input)   { return match_exact_kw(input, "return"); }

