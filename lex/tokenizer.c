/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 21:49:46 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/05 01:44:10 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "ds.h"
#include <stdint.h> /* added for uintptr_t */

static char *parse_squote(t_deque *tokens, char **str)
{
	char *start = *str;

	ft_assert(**str == '\'');
	(*str)++; /* consume opening quote */

	while (**str && **str != '\'')
		(*str)++;

	if (**str != '\'')
		return (LEXER_SQUOTE_PROMPT); /* unclosed */

	(*str)++; /* consume closing quote */

	deque_push_end(tokens, &(t_token){
							   .start = start + 1,		/* skip opening quote */
							   .len = *str - start - 2, /* exclude both quotes */
							   .type = TOKEN_SQUOTE_STRING});
	return (0);
}

static char *parse_dquote(t_deque *tokens, char **str)
{
	char *start = *str;
	bool prev_bs = false;

	ft_assert(**str == '"');
	(*str)++; /* consume opening quote */

	while (**str && (**str != '"' || prev_bs))
	{
		prev_bs = **str == '\\' && !prev_bs;
		(*str)++;
	}

	if (**str != '"')
		return (LEXER_DQUOTE_PROMPT); /* unclosed */

	(*str)++; /* consume closing quote */

	deque_push_end(tokens, &(t_token){
							   .start = start + 1,		/* skip opening quote */
							   .len = *str - start - 2, /* exclude both quotes */
							   .type = TOKEN_DQUOTE_STRING});
	return (0);
}

char *parse_word(t_deque *tokens, char **str)
{
	char *start;

	start = *str;
	while (**str)
	{
		if (**str == '\\')
			advance_bs(str);
		else if (!is_spechr(**str) && **str != '\'' && **str != '"' && **str != '$')
			(*str)++;
		else
			break;
	}

	/* Only emit WORD if we consumed something */
	if (*str > start)
	{
		deque_push_end(
			tokens, &(t_token){.start = start, .len = *str - start, .type = TOKEN_WORD});
	}
	return (0);
}

void parse_op(t_deque *tokens, char **str)
{
	char *start;
	int op_idx;
	t_op_map operators[13];
	t_op_map sample;
	size_t offset;

	operators[0] = (t_op_map){"|", TOKEN_PIPE};
	operators[1] = (t_op_map){"<<", TOKEN_REDIR_HEREDOC};
	operators[2] = (t_op_map){"<<-", TOKEN_REDIR_CLOBBER};
	operators[3] = (t_op_map){">>", TOKEN_REDIR_APPEND};
	operators[4] = (t_op_map){"(", TOKEN_RIGHT_PAREN};
	operators[5] = (t_op_map){")", TOKEN_LEFT_PAREN};
	operators[6] = (t_op_map){">", TOKEN_REDIR_OUT};
	operators[7] = (t_op_map){"<", TOKEN_REDIR_IN};
	operators[8] = (t_op_map){"&&", TOKEN_LOGICAL_AND};
	operators[9] = (t_op_map){"&", TOKEN_AMPERSAND};
	operators[10] = (t_op_map){"||", TOKEN_LOGICAL_OR};
	operators[11] = (t_op_map){";", TOKEN_SEMICOLON};
	operators[12] = (t_op_map){0, TOKEN_END};
	start = *str;
	offset = ft_offsetof(&sample, &sample.lexem);
	op_idx = max_munch(*str, operators, offset, sizeof(t_op_map));
	ft_assert(op_idx != -1);
	*str += ft_strlen(operators[op_idx].lexem);
	deque_push_end(tokens, &(t_token){
							   .type = operators[op_idx].type,
							   .start = start,
							   .len = *str - start,
						   });
}

char *tokenizer(char *str, t_deque *ret)
{
	char *prompt;

	prompt = 0;
	deque_clear(ret);
	while (str && *str)
	{
		/* Handle quotes separately */
		if (*str == '\'')
			prompt = parse_squote(ret, &str);
		else if (*str == '"')
			prompt = parse_dquote(ret, &str);
		else if (*str == '$' || !(is_spechr(*str)))
			prompt = parse_word(ret, &str);
		else if (*str == '\n')
		{
			deque_push_end(ret, &(t_token){
									.type = TOKEN_NEWLINE,
									.start = str,
									.len = 1,
									.allocated = false});
			str++;
		}
		else if (ft_isspace(*str))
			str++;
		else
			parse_op(ret, &str);
		if (prompt)
			break;
	}
	deque_push_end(ret, &(t_token){
							.type = TOKEN_END,
						});
	return (prompt);
}
