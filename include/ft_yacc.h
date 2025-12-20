/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_yacc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 14:31:50 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/20 02:21:08 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_YACC_H
# define FT_YACC_H

#include "../include/internals/lexer.h"
#include "../include/internals/dyn_string.h"
#include "../include/internals/ft_vector.h"
#include "../include/ft_ctype.h"
#include "../include/ft_debug.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * t_token type is already present into lexer.h so we don't need to recreate it
 */
 
typedef struct s_position
{
	size_t	line;
	size_t	column;
	size_t	idx;
}	t_position;

//ytoken, because we already have another struct called token
typedef struct s_ytoken
{
	t_token_type	type;
	t_dyn_str		input;
	t_position		position;
}	t_ytoken;

/* forward declare the yacc struct so token-level transform typedef can refer to it */
struct s_yacc;

typedef bool (*t_matchfn)(t_dyn_str *input);
typedef void (*t_transfn)(t_dyn_str value);
/* token-level transform may refer to the yacc instance as struct s_yacc */
typedef void (*t_trans_token_fn)(t_ytoken *token, struct s_yacc *yacc, size_t before_pos);

typedef struct s_yrule
{
	t_matchfn		match;
	t_transfn		transform;
	/* new optional token-level transform (preferred for merge/exception rules) */
	t_trans_token_fn	transform_token;
	t_token_type	type;
}t_yrule;

typedef struct s_yopt
{
	bool	skip_whitespace;
	bool	track_position;
	bool	error_on_unknown;
}t_yopt;

/**
 * those are the new rule yacc, that need to be used with structure
 * t_vec standsd for vecctorization
 */
typedef struct s_yacc
{
	t_dyn_str	input;
	t_position	pos;
	t_vec		rules;
	t_yopt		options;
	int			arith_depth; /* new: tracks arithmetic expression nesting */
	int			subshell_depth; /* new: tracks subshell (e.g. $()) nesting */
}	t_yacc;

/* Allow transforms (t_transfn) to access the active parser instance during transform.
   Implementation sets a small current pointer around transform calls. */
void	yacc_set_current(t_yacc *yacc);
t_yacc	*yacc_get_current(void);

t_yacc *yacc_create(t_yopt options);
int yacc_init(t_yacc *yacc);
t_yacc *get_yacc(t_yacc *maybe);
void yacc_add_rule(t_yacc *yacc, t_matchfn match_fn, t_token_type type_en, t_transfn trans_fn);
/* new: register rule with token-level transform */
void add_grammar_token(t_yacc *yacc, t_matchfn match_fn, t_token_type type_en, t_trans_token_fn trans_tok);
void yacc_skip_whitespace(t_yacc *yacc);
void yacc_update_position(t_yacc *yacc, size_t length);
bool match_comment(t_dyn_str *input);
bool match_identifier(t_dyn_str *input);
bool match_number(t_dyn_str *input);
bool match_sqstring(t_dyn_str *input);
bool match_dqstring(t_dyn_str *input);
bool match_variable(t_dyn_str *input);
bool match_keyword(t_dyn_str *input);
bool match_operator(t_dyn_str *input);
bool match_pipe(t_dyn_str *input);
bool match_and(t_dyn_str *input);
bool match_or(t_dyn_str *input);
bool	match_ampersand(t_dyn_str *input);
t_ytoken *yacc_next_token(t_yacc *yacc);
t_ytoken **yacc_tokenize_all(t_yacc *yacc, int *token_count);
t_yacc *yacc_create(t_yopt options);
void yacc_destroy(t_yacc *yacc);
void yacc_setup_deft_lexer(t_yacc *yacc);
bool	match_word(t_dyn_str *input);

/* Arithmetic / subshell context helpers */
void	yacc_enter_arith(t_yacc *yacc);
void	yacc_exit_arith(t_yacc *yacc);
int		yacc_in_arith(t_yacc *yacc);
t_token_type	yacc_remap_arith_token(t_token_type type);

void	yacc_enter_subshell(t_yacc *yacc);
void	yacc_exit_subshell(t_yacc *yacc);
int		yacc_in_subshell(t_yacc *yacc);
/* Forward declarations for per-keyword matchers implemented in
   collec_handlers/match_keyword.c so yacc_setup_deft_lexer can register them. */
bool match_kw_if(t_dyn_str *input);
bool match_kw_then(t_dyn_str *input);
bool match_kw_else(t_dyn_str *input);
bool match_kw_elif(t_dyn_str *input);
bool match_kw_fi(t_dyn_str *input);
bool match_kw_for(t_dyn_str *input);
bool match_kw_while(t_dyn_str *input);
bool match_kw_do(t_dyn_str *input);
bool match_kw_done(t_dyn_str *input);
bool match_kw_case(t_dyn_str *input);
bool match_kw_esac(t_dyn_str *input);
bool match_kw_in(t_dyn_str *input);
bool match_kw_function(t_dyn_str *input);
bool match_kw_select(t_dyn_str *input);
bool match_kw_return(t_dyn_str *input);

/* Forward declarations for new grouping matchers (defined in collec_handlers) */
bool match_lparen(t_dyn_str *input);
bool match_rparen(t_dyn_str *input);
bool match_lbrace(t_dyn_str *input);
bool match_rbrace(t_dyn_str *input);
bool match_semicolon(t_dyn_str *input);
bool match_lbracket(t_dyn_str *input); /* added */
bool match_rbracket(t_dyn_str *input); /* added */

/* --- new: forward declarations for $-paren matchers --- */
bool match_dollar_arith(t_dyn_str *input); /* $(( */
bool match_dollar_cmd(t_dyn_str *input);   /* $(  */

/* forward declarations for double-paren subshell matchers */
bool match_double_lparen(t_dyn_str *input); /* (( */
bool match_double_rparen(t_dyn_str *input); /* )) */

#ifdef __cplusplus
}
#endif

#endif
