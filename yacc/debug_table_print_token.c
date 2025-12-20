/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_table_print_token.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 18:04:43 by marvin            #+#    #+#             */
/*   Updated: 2025/12/19 18:04:43 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "ft_colors.h"

const char	*get_token_name(t_token_type type);
const char	*token_color(t_token_type type);
void		print_token_debug(t_token *token);

static void	helper_start_print_tokens(t_deque *tokens)
{
	printf("\n");
	printf(ASCII_CYAN "╔═══════════════════════════════════════"
		"═════════════════════════════════════╗\n" RESET_TERM);
	printf(ASCII_CYAN "║" RESET_TERM BOLD_TERM
		"%42s%34s"
		RESET_TERM ASCII_CYAN "║\n" RESET_TERM, "TOKEN_LIST", "");
	printf(ASCII_CYAN "╚═══════════════════════════════════════"
		"═════════════════════════════════════╝\n" RESET_TERM);
	printf(ASCII_GREY "Total tokens: %d\n\n" RESET_TERM, (int)tokens->len);
	printf(ASCII_CYAN "╔═════╦═══════════════════════════"
		"═════╦═════╦════════════════════════════════╗\n" RESET_TERM);
	printf(ASCII_CYAN "║" RESET_TERM BOLD_TERM " idx "
		RESET_TERM ASCII_CYAN "║" RESET_TERM BOLD_TERM " %-30s "
		RESET_TERM ASCII_CYAN "║" RESET_TERM BOLD_TERM " len "
		RESET_TERM ASCII_CYAN "║" RESET_TERM BOLD_TERM " %-30s "
		RESET_TERM ASCII_CYAN "║\n" RESET_TERM, "type", "lexeme");
	printf(ASCII_CYAN "╠═════╬════════════════════════════════╬══"
		"═══╬════════════════════════════════╣\n" RESET_TERM);
}

static void	helper_end_print_tokens(void)
{
	printf(ASCII_CYAN "╚═════╩═════════════════════════"
		"═══════╩═════╩════════════════════════════════╝\n" RESET_TERM);
	printf("\n");
}

static void	help_process_print_tokens(t_deque *tokens, t_token *curr,
				t_dyn_str lexem)
{
	int		i;
	char	lexeme_buf[128];

	i = -1;
	while (++i < (int)tokens->len)
	{
		curr = (t_token *)deque_idx(tokens, i);
		lexem.buff = (char *)get_token_name(curr->type);
		lexem.data = (void *)token_color(curr->type);
		if (curr->len > 0)
			snprintf(lexeme_buf, sizeof(lexeme_buf), "'%.*s'",
				curr->len, curr->start);
		else
			snprintf(lexeme_buf, sizeof(lexeme_buf), "(empty)");
		printf(ASCII_CYAN "║" RESET_TERM " %3d " ASCII_CYAN "║"
			RESET_TERM " %s%-30s%s "ASCII_CYAN "║" RESET_TERM " %3d "
			ASCII_CYAN "║" RESET_TERM " %s%-30s%s"
			ASCII_CYAN " ║\n" RESET_TERM,
			i, (char *)lexem.data, lexem.buff, RESET_TERM, curr->len,
			ASCII_GREY, lexeme_buf, RESET_TERM);
	}
}

void	print_tokens(t_deque *tokens)
{
	t_token		*curr;
	t_dyn_str	lexem;

	curr = NULL;
	lexem = (t_dyn_str){0};
	helper_start_print_tokens(tokens);
	help_process_print_tokens(tokens, curr, lexem);
	helper_end_print_tokens();
}
