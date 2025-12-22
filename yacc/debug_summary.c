/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_summary.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 18:03:24 by marvin            #+#    #+#             */
/*   Updated: 2025/12/19 18:03:24 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "ft_colors.h"

const char	*get_token_name(t_token_type type);
const char	*token_color(t_token_type type);

static void	helper_summary(int total)
{
	printf("\n");
	printf(ASCII_MAGENTA "╔═══════════════════════════"
		"═════════════════════╗\n" RESET_TERM);
	printf(ASCII_MAGENTA "║" RESET_TERM BOLD_TERM
		"              TOKEN SUMMARY                     "
		RESET_TERM ASCII_MAGENTA "║\n" RESET_TERM);
	printf(ASCII_MAGENTA "╚════════════════════════"
		"════════════════════════╝\n" RESET_TERM);
	printf(ASCII_GREY "Total tokens: %d\n\n" RESET_TERM, total);
	printf(ASCII_MAGENTA "╔════════════════════"
		"════════════╦═══════╗\n" RESET_TERM);
	printf(ASCII_MAGENTA "║" RESET_TERM BOLD_TERM " %-30s "
		RESET_TERM ASCII_MAGENTA "║" RESET_TERM BOLD_TERM
		" count " RESET_TERM ASCII_MAGENTA
		"║\n" RESET_TERM, "type");
	printf(ASCII_MAGENTA "╠═════════════════════"
		"═══════════╬═══════╣\n" RESET_TERM);
}

static void	help_process_summary(t_dyn_str lexem,
				int type_counts[TOKEN_SENTINEL])
{
	int	i;

	i = -1;
	while (++i < TOKEN_SENTINEL)
	{
		if (type_counts[i] > 0)
		{
			lexem.buff = (char *)get_token_name(i);
			lexem.data = (void *)token_color(i);
			printf(ASCII_MAGENTA "║" RESET_TERM " %s%-30s%s " ASCII_MAGENTA
				"║" RESET_TERM " %5d " ASCII_MAGENTA "║\n" RESET_TERM,
				(char *)lexem.data, lexem.buff, RESET_TERM, type_counts[i]);
		}
	}
}

void	print_token_summary(t_deque *tokens)
{
	const int	total = (int)tokens->len;
	t_token		*curr;
	int			type_counts[TOKEN_SENTINEL];
	int			i;

	ft_memset(type_counts, 0, sizeof(type_counts));
	i = -1;
	while (++i < (int)tokens->len)
	{
		curr = (t_token *)deque_idx(tokens, i);
		if (curr->type < TOKEN_SENTINEL)
			type_counts[curr->type]++;
	}
	helper_summary(total);
	help_process_summary((t_dyn_str){0}, type_counts);
	printf(ASCII_MAGENTA "╚════════════════════════════════╩═══════╝\n"
		RESET_TERM);
	printf("\n");
}
