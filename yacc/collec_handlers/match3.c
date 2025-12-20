/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   match5.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 22:24:03 by marvin            #+#    #+#             */
/*   Updated: 2025/12/19 22:24:03 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "ft_yacc.h"
extern t_yacc *yacc_get_current(void);
extern void yacc_enter_arith(t_yacc *yacc);
extern void yacc_enter_subshell(t_yacc *yacc);
extern void yacc_exit_subshell(t_yacc *yacc);
extern int yacc_in_subshell(t_yacc *yacc);
extern int yacc_in_arith(t_yacc *yacc);

bool	match_pipe(t_dyn_str *input)
{
	const size_t	pos = input->pos;

	ft_assert(input && input->buff);
	if (pos >= input->len || dyn_str_idx(input, pos) != '|')
		return (false);
	if (pos + 1 < input->len && dyn_str_idx(input, pos + 1) == '|')
		return (false);
	input->pos = pos + 1;
	return (true);
}

bool	match_and(t_dyn_str *input)
{
	const size_t	pos = input->pos;

	ft_assert(input && input->buff);
	if (pos >= input->len || dyn_str_idx(input, pos) != '&')
		return (false);
	if (pos + 1 < input->len && dyn_str_idx(input, pos + 1) == '&')
	{
		input->pos = pos + 2;
		return (true);
	}
	return (false);
}

bool	match_or(t_dyn_str *input)
{
	const size_t	pos = input->pos;

	ft_assert(input && input->buff);
	if (pos >= input->len || dyn_str_idx(input, pos) != '|')
		return (false);
	if (pos + 1 < input->len && dyn_str_idx(input, pos + 1) == '|')
	{
		input->pos = pos + 2;
		return (true);
	}
	return (false);
}

bool	match_ampersand(t_dyn_str *input)
{
	const size_t	pos = input->pos;

	ft_assert(input && input->buff);
	if (pos >= input->len || dyn_str_idx(input, pos) != '&')
		return (false);
	if (pos + 1 < input->len && dyn_str_idx(input, pos + 1) == '&')
		return (false);
	input->pos = pos + 1;
	return (true);
}

static int is_metachar_wrap(int c)
{
	return ((int)is_metachar(c));
}

bool	match_word(t_dyn_str *input)
{
	const size_t	pos = input->pos;
	char			c;

	ft_assert(input && input->buff);
	if (pos >= input->len)
		return (false);
	/* don't start a word on whitespace or metacharacters */
	if (ft_isspace(dyn_str_idx(input, pos))
		|| dyn_str_check_at(input, pos, is_metachar_wrap, NULL))
		return (false);
	while (input->pos < input->len)
	{
		c = dyn_str_idx(input, input->pos);
		/* stop on whitespace, metachar, or explicit assignment/colon separators */
		if (ft_isspace(c) || is_metachar(c) || c == '=' || c == ':')
			break ;
		input->pos++;
	}
	return (true);
}

/* --- new: match single-char grouping / punctuation tokens --- */
bool match_lparen(t_dyn_str *input)
{
	const size_t pos = input->pos;
	ft_assert(input && input->buff);
	if (pos >= input->len || dyn_str_idx(input, pos) != '(')
		return (false);
	input->pos = pos + 1;
	return (true);
}

bool match_rparen(t_dyn_str *input)
{
	const size_t pos = input->pos;
	ft_assert(input && input->buff);
	if (pos >= input->len || dyn_str_idx(input, pos) != ')')
		return (false);
	input->pos = pos + 1;
	return (true);
}

bool match_lbrace(t_dyn_str *input)
{
	const size_t pos = input->pos;
	ft_assert(input && input->buff);
	if (pos >= input->len || dyn_str_idx(input, pos) != '{')
		return (false);
	input->pos = pos + 1;
	return (true);
}

bool match_rbrace(t_dyn_str *input)
{
	const size_t pos = input->pos;
	ft_assert(input && input->buff);
	if (pos >= input->len || dyn_str_idx(input, pos) != '}')
		return (false);
	input->pos = pos + 1;
	return (true);
}

bool match_semicolon(t_dyn_str *input)
{
	const size_t pos = input->pos;
	ft_assert(input && input->buff);
	if (pos >= input->len || dyn_str_idx(input, pos) != ';')
		return (false);
	input->pos = pos + 1;
	return (true);
}

bool match_lbracket(t_dyn_str *input)
{
	const size_t pos = input->pos;
	ft_assert(input && input->buff);
	if (pos >= input->len || dyn_str_idx(input, pos) != '[')
		return (false);
	input->pos = pos + 1;
	return (true);
}

bool match_rbracket(t_dyn_str *input)
{
	const size_t pos = input->pos;
	ft_assert(input && input->buff);
	if (pos >= input->len || dyn_str_idx(input, pos) != ']')
		return (false);
	input->pos = pos + 1;
	return (true);
}

/* --- new: match $((   and $(   --- */
bool match_dollar_arith(t_dyn_str *input)
{
	const size_t pos = input->pos;
	t_yacc *y;

	ft_assert(input && input->buff);
	if (pos + 2 >= input->len)
		return (false);
	if (dyn_str_idx(input, pos) == '$'
		&& dyn_str_idx(input, pos + 1) == '('
		&& dyn_str_idx(input, pos + 2) == '(')
	{
		input->pos = pos + 3; /* consume $(( */
		y = yacc_get_current();
		if (y)
			yacc_enter_arith(y);
		return (true);
	}
	return (false);
}

bool match_dollar_cmd(t_dyn_str *input)
{
	const size_t pos = input->pos;
	t_yacc *y;

	ft_assert(input && input->buff);
	if (pos + 1 >= input->len)
		return (false);
	if (dyn_str_idx(input, pos) == '$'
		&& dyn_str_idx(input, pos + 1) == '('
		&& !(pos + 2 < input->len && dyn_str_idx(input, pos + 2) == '('))
	{
		input->pos = pos + 2; /* consume $( */
		y = yacc_get_current();
		if (y)
			yacc_enter_subshell(y);
		return (true);
	}
	return (false);
}

/* --- new: match double-left '((' as arithmetic-eval start (enter arith) --- */
bool match_double_lparen(t_dyn_str *input)
{
	const size_t pos = input->pos;
	size_t i;
	t_yacc *y;
	bool found_close = false;
	bool is_command = false;

	ft_assert(input && input->buff);
	if (pos + 1 >= input->len)
		return (false);
	if (!(dyn_str_idx(input, pos) == '(' && dyn_str_idx(input, pos + 1) == '('))
		return (false);

	/* Look ahead to find a matching '))' and analyze the inner content.
	   Detect if this looks like a command (not arithmetic):
	   - Contains pipes (|), semicolons (;), redirections (<, >)
	   - Word followed by hyphen-flag pattern (e.g., "cat -e")
	   - Contains quotes (suggests command arguments)
	*/
	i = pos + 2;
	while (i + 1 < input->len)
	{
		char c = dyn_str_idx(input, i);
		
		/* Hard reject on shell separators */
		if (c == '|' || c == ';' || c == '<' || c == '>')
			return (false);
		
		/* Check for command pattern: identifier/word followed by space then hyphen */
		if ((ft_isalpha((int)c) || c == '_'))
		{
			size_t j = i;
			while (j < input->len && 
				   (ft_isalnum((int)dyn_str_idx(input, j)) || dyn_str_idx(input, j) == '_'))
				j++;
			/* After word, check for space + hyphen (command option) */
			if (j < input->len && dyn_str_idx(input, j) == ' ')
			{
				size_t k = j + 1;
				while (k < input->len && dyn_str_idx(input, k) == ' ')
					k++;
				if (k < input->len && dyn_str_idx(input, k) == '-')
				{
					is_command = true;
					break;
				}
			}
			i = j;
			continue;
		}
		
		/* Quotes in arithmetic are rare; likely a command string */
		if (c == '"' || c == '\'' || c == '`')
		{
			is_command = true;
			break;
		}
		
		if (c == ')' && dyn_str_idx(input, i + 1) == ')')
		{
			found_close = true;
			break;
		}
		i++;
	}
	
	if (!found_close || is_command)
		return (false);

	/* Safe to treat as arithmetic */
	input->pos = pos + 2; /* consume '((' */
	y = yacc_get_current();
	if (y)
		yacc_enter_arith(y);
	return (true);
}

/* --- new: match double-right '))' as arithmetic end (exit arith) --- */
bool match_double_rparen(t_dyn_str *input)
{
	const size_t pos = input->pos;
	t_yacc *y;

	ft_assert(input && input->buff);
	if (pos + 1 >= input->len)
		return (false);
	if (dyn_str_idx(input, pos) == ')' && dyn_str_idx(input, pos + 1) == ')')
	{
		y = yacc_get_current();
		if (y && yacc_in_arith(y))
		{
			input->pos = pos + 2; /* consume '))' */
			yacc_exit_arith(y); /* exit arithmetic context */
			return (true);
		}
	}
	return (false);
}
