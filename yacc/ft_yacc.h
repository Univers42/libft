/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_yacc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 14:31:50 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/19 04:05:07 by dlesieur         ###   ########.fr       */
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

typedef bool (*match_fn)(t_dyn_str *input);
typedef void (*trans_fn)(t_dyn_str value);

typedef struct s_yrule
{
	match_fn		match;
	trans_fn		transform;
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


#endif
