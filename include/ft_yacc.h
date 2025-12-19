/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_yacc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 14:31:50 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/19 05:25:54 by dlesieur         ###   ########.fr       */
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

typedef bool (*t_matchfn)(t_dyn_str *input);
typedef void (*t_transfn)(t_dyn_str value);

typedef struct s_yrule
{
	t_matchfn		match;
	t_transfn		transform;
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
	t_vec		rules;		//we vectorize rule instead of all teh structure
	t_yopt		options;
}	t_yacc;

#ifdef __cplusplus
}

t_yacc *yacc_create(t_yopt options);
int yacc_init(t_yacc *yacc);
t_yacc *get_yacc(t_yacc *maybe);
void yacc_add_rule(t_yacc *yacc, t_matchfn match_fn, t_token_type type_en, t_transfn trans_fn);
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
t_ytoken *yacc_next_token(t_yacc *yacc);
t_ytoken **yacc_tokenize_all(t_yacc *yacc, int *token_count);
t_yacc *yacc_create(t_yopt options);
void yacc_destroy(t_yacc *yacc);
void yacc_setup_deft_lexer(t_yacc *yacc);

#endif

#endif
