/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   yacc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 14:40:33 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/20 02:20:58 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_yacc.h"
#include <string.h>
#include <ctype.h>



/* small global pointer used only during transform callbacks */
static t_yacc *g_current_yacc = NULL;

/* expose simple accessors (declared in header) */
void	yacc_set_current(t_yacc *yacc)
{
	g_current_yacc = yacc;
}

t_yacc	*yacc_get_current(void)
{
	return g_current_yacc;
}

/**
 * Singleton accessor for the default yacc parser
 * Returns either the provided pointer or the static singleton
 */
t_yacc	*get_yacc(t_yacc *maybe)
{
	static	t_yacc singleton = {0};
	
	if (maybe)
		return maybe;
	return &singleton;
}

/**
 * Initialize the yacc parser
 * Can be called with NULL for singleton or with a custom structure
 */
int	yacc_init(t_yacc *yacc)
{
	t_yacc *y;

	y = get_yacc(yacc);
	ft_assert(y != NULL);
	y->input = (t_dyn_str){0};
	y->pos = (t_position){0};
	y->options = (t_yopt){0};
	y->arith_depth = 0; /* new: initialize arithmetic context */
	vec_init(&y->rules, &(t_vec_config){
		.elem_size = sizeof(t_yrule),
		.initial_capacity = 64,
		.type_mask = VEC_TYPE_PLAIN
	});
	return 0;
}

/**
 * Add a rule to the yacc parser
 * Works with both singleton and custom structures
 */
void	add_grammar(t_yacc *yacc, t_matchfn match_fn,
			t_token_type type_en, t_transfn trans_fn)
{
	t_yacc	*y;
	t_yrule	rule;

	y = get_yacc(yacc);
	ft_assert(y != NULL);
	rule.match = match_fn;
	rule.type = type_en;
	rule.transform = trans_fn;
	rule.transform_token = NULL; /* ensure new field is NULL for backward compat */
	vec_push(&y->rules, &rule);
}

/* new: register rule with token-level transform callback */
void add_grammar_token(t_yacc *yacc, t_matchfn match_fn,
			t_token_type type_en, t_trans_token_fn trans_tok)
{
	t_yacc	*y;
	t_yrule	rule;

	y = get_yacc(yacc);
	ft_assert(y != NULL);
	rule.match = match_fn;
	rule.type = type_en;
	rule.transform = NULL;
	rule.transform_token = trans_tok;
	vec_push(&y->rules, &rule);
}


/**
 * Skip whitespace in the input
 */
void	yacc_skip_whitespace(t_yacc *yacc)
{
	ft_assert(yacc != NULL);
	while (yacc->input.pos < yacc->input.len && 
		   ft_isspace(dyn_str_idx(&yacc->input, yacc->input.pos)))
		yacc->input.pos++;
}

/**
 * Update position tracking (line, column, index)
 */
void	yacc_update_position(t_yacc *yacc, size_t length)
{
	size_t	i;
	
	ft_assert(yacc != NULL);
	i = 0;
	while (i < length && yacc->pos.idx < yacc->input.len)
	{
		if (dyn_str_idx(&yacc->input, yacc->pos.idx) == '\n')
		{
			yacc->pos.line++;
			yacc->pos.column = 0;
		}
		else
			yacc->pos.column++;
		yacc->pos.idx++;
		i++;
	}
}




/**
 * Get next token from the input
 */
t_ytoken	*yacc_next_token(t_yacc *yacc)
{
	size_t		i;
	t_yrule		*rule;
	t_ytoken	*token;
	size_t		before;
	size_t		consumed;

	ft_assert(yacc != NULL);
	if (yacc->options.skip_whitespace)
		yacc_skip_whitespace(yacc);
	if (yacc->input.pos >= yacc->input.len)
		return (NULL);

	before = yacc->input.pos;

	/* Ensure matchers can access parser state during speculative matching */
	yacc_set_current(yacc);

	/* Try every rule speculatively using a temp copy of the input,
	   pick the rule that consumes the most characters (maximal munch). */
	size_t best_consumed = 0;
	t_yrule *best_rule = NULL;
	for (i = 0; i < yacc->rules.len; i++)
	{
		rule = (t_yrule *)vec_idx(&yacc->rules, i);
		if (!rule || !rule->match)
			continue;
		/* speculative copy of input */
		t_dyn_str tmp = yacc->input;
		tmp.pos = before;
		if (rule->match(&tmp))
		{
			size_t c = tmp.pos - before;
			if (c > best_consumed)
			{
				best_consumed = c;
				best_rule = rule;
			}
		}
	}

	/* No rule matched */
	if (!best_rule)
	{
		yacc_set_current(NULL);
		return (NULL);
	}

	/* If matched but consumed 0 (defensive), advance by one to avoid infinite loop */
	consumed = best_consumed;
	if (consumed == 0)
	{
		yacc->input.pos = before + 1;
		consumed = 1;
	}
	else
	{
		/* commit the consumption */
		yacc->input.pos = before + consumed;
	}

	/* update position bookkeeping for the initial consumption */
	yacc_update_position(yacc, consumed);

	/* build token */
	token = malloc(sizeof(t_ytoken));
	if (!token)
	{
		yacc_set_current(NULL);
		return (NULL);
	}
	token->type = best_rule->type;
	token->position = yacc->pos;
	token->input = yacc->input;

	/* legacy transform that works on dyn_str (kept for compatibility)
	   Set current yacc so transforms can access/modify parser state. */
	if (best_rule->transform)
	{
		/* current already set */
		best_rule->transform(token->input);
	}

	/* new token-level transform: can inspect/extend consumption and update yacc state */
	if (best_rule->transform_token)
	{
		best_rule->transform_token(token, yacc, before);
		token->position = yacc->pos;
		token->input = yacc->input;
	}

	/* clear current pointer before returning */
	yacc_set_current(NULL);
	return token;
}

/**
 * Tokenize entire input
 */
t_ytoken	**yacc_tokenize_all(t_yacc *yacc, int *token_count)
{
	int			cap;
	int			count;
	t_ytoken	**tokens;
	t_ytoken	*token;

	ft_assert(yacc && token_count);
	cap = 64;
	count = 0;
	tokens = malloc(sizeof(t_ytoken *) * cap);
	if (!tokens)
		return (NULL);
	while (yacc->pos.idx < yacc->input.len)
	{
		token = yacc_next_token(yacc);
		if (!token)
			break ;
		if (count >= cap)
		{
			cap *= 2;
			tokens = realloc(tokens, sizeof(t_ytoken *) * cap);
			if (!tokens)
				return (NULL);
		}
		tokens[count++] = token;
	}
	*token_count = count;
	return (tokens);
}

/**
 * Create a yacc parser with custom options
 */
t_yacc	*yacc_create(t_yopt options)
{
	t_yacc	*yacc;

	yacc = malloc(sizeof(t_yacc));
	if (!yacc)
		return (NULL);
	yacc_init(yacc);
	yacc->options = options;
	return (yacc);
}

/**
 * Destroy a yacc parser
 */
void yacc_destroy(t_yacc *yacc)
{
	if (!yacc)
		return ;
	dyn_str_free(&yacc->input);
	vec_destroy(&yacc->rules);
	free(yacc);
}

/* --- new: match parameter expansion start "${" --- */
static bool match_par_exp_start(t_dyn_str *input)
{
	const size_t pos = input->pos;
	ft_assert(input && input->buff);
	if (pos + 1 >= input->len)
		return (false);
	if (dyn_str_idx(input, pos) == '$' && dyn_str_idx(input, pos + 1) == '{')
	{
		input->pos = pos + 2; /* consume '${' and leave inner content to lexer */
		return (true);
	}
	return (false);
}

/* --- updated: match single-char special variable forms only (e.g. $?, $$, $@) --- */
static bool match_special_variable(t_dyn_str *input)
{
	const size_t pos = input->pos;

	ft_assert(input && input->buff);
	if (pos >= input->len || dyn_str_idx(input, pos) != '$')
		return (false);
	/* do not consume '${' here (handled by match_par_exp_start) */
	if (pos + 1 < input->len && dyn_str_idx(input, pos + 1) == '{')
		return (false);
	/* single-char special params like $*, $#, $@, $?, $$, $!, $-, $0 */
	if (pos + 1 < input->len)
	{
		char c = dyn_str_idx(input, pos + 1);
		if (strchr("*#@?$!-0", c) || c == '$')
		{
			input->pos = pos + 2;
			return (true);
		}
	}
	return (false);
}


/**
 * Setup default lexer rules
 * ORDER MATTERS, MORE SPECIFIC RULES
 */
void yacc_setup_deft_lexer(t_yacc *yacc)
{
	t_yacc *y = get_yacc(yacc);

	// Comments first
	add_grammar(y, match_comment, TOKEN_HASH, NULL);

	/* $(( and $( must be recognized before ${ and other $-forms */
	add_grammar(y, match_dollar_arith, TOKEN_ARITH_EXP_START, NULL);
	add_grammar(y, match_dollar_cmd, TOKEN_CMD_SUBST_START, NULL);

	// parameter expansion start "${" before other $-forms
	add_grammar(y, match_par_exp_start, TOKEN_PAR_EXP_START, NULL);

	// special variables (e.g. $?, $$) before generic variable
	add_grammar(y, match_special_variable, TOKEN_VARIABLE, NULL);

	/* register specific keyword matchers so each rule carries the exact token type */
	add_grammar(y, match_kw_if, TOKEN_IF, NULL);
	add_grammar(y, match_kw_then, TOKEN_THEN, NULL);
	add_grammar(y, match_kw_else, TOKEN_ELSE, NULL);
	add_grammar(y, match_kw_elif, TOKEN_ELIF, NULL);
	add_grammar(y, match_kw_fi, TOKEN_FI, NULL);
	add_grammar(y, match_kw_for, TOKEN_FOR, NULL);
	add_grammar(y, match_kw_while, TOKEN_WHILE, NULL);
	add_grammar(y, match_kw_do, TOKEN_DO, NULL);
	add_grammar(y, match_kw_done, TOKEN_DONE, NULL);
	add_grammar(y, match_kw_case, TOKEN_CASE, NULL);
	add_grammar(y, match_kw_esac, TOKEN_ESAC, NULL);
	add_grammar(y, match_kw_return, TOKEN_RETURN, NULL);
	add_grammar(y, match_kw_in, TOKEN_IN, NULL);
	add_grammar(y, match_kw_function, TOKEN_FUNCTION, NULL);
	add_grammar(y, match_kw_select, TOKEN_SELECT, NULL);

	/* grouping / punctuation (must come before operator/identifier/word) */
	/* try double-right and double-left before single parens */
	add_grammar(y, match_double_rparen, TOKEN_ARITH_EXP_END, NULL);
	add_grammar(y, match_double_lparen, TOKEN_ARITH_EXP_START, NULL);
	add_grammar(y, match_lparen, TOKEN_LEFT_PAREN, NULL);
	add_grammar(y, match_rparen, TOKEN_RIGHT_PAREN, NULL);
	add_grammar(y, match_lbrace, TOKEN_LEFT_BRACE, NULL);
	add_grammar(y, match_rbrace, TOKEN_RIGHT_BRACE, NULL);
	add_grammar(y, match_lbracket, TOKEN_GLOB_BRACKET_OPEN, NULL);
	add_grammar(y, match_rbracket, TOKEN_GLOB_BRACKET_CLOSE, NULL);
	add_grammar(y, match_semicolon, TOKEN_SEMICOLON, NULL);

	/* generic variable fallback and others (order: variable, number, strings, ops, ids, words) */
	add_grammar(y, match_variable, TOKEN_VARIABLE, NULL);
	add_grammar(y, match_number, TOKEN_NUMBER, NULL);
	add_grammar(y, match_sqstring, TOKEN_SQUOTE_STRING, NULL);
	add_grammar(y, match_dqstring, TOKEN_DQUOTE_STRING, NULL);
	add_grammar(y, match_or, TOKEN_LOGICAL_OR, NULL);
	add_grammar(y, match_and, TOKEN_LOGICAL_AND, NULL);
	add_grammar(y, match_pipe, TOKEN_PIPE, NULL);
	add_grammar(y, match_ampersand, TOKEN_AMPERSAND, NULL);
	add_grammar(y, match_operator, TOKEN_OPERATOR, NULL);
	add_grammar(y, match_identifier, TOKEN_IDENTIFIER, NULL);
	add_grammar(y, match_word, TOKEN_WORD, NULL);
}

