/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arith_context.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 01:45:00 by marvin            #+#    #+#             */
/*   Updated: 2025/12/20 01:45:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_yacc.h"

/**
 * Enter an arithmetic expression context
 * Increments the nesting depth
 */
void	yacc_enter_arith(t_yacc *yacc)
{
	ft_assert(yacc != NULL);
	yacc->arith_depth++;
}

/**
 * Exit an arithmetic expression context
 * Decrements the nesting depth (floor at 0)
 */
void	yacc_exit_arith(t_yacc *yacc)
{
	ft_assert(yacc != NULL);
	if (yacc->arith_depth > 0)
		yacc->arith_depth--;
}

/**
 * Check if currently inside an arithmetic expression
 */
int	yacc_in_arith(t_yacc *yacc)
{
	ft_assert(yacc != NULL);
	return (yacc->arith_depth > 0);
}

/* Subshell context helpers (for $() nesting) */

void	yacc_enter_subshell(t_yacc *yacc)
{
	ft_assert(yacc != NULL);
	yacc->subshell_depth++;
}

void	yacc_exit_subshell(t_yacc *yacc)
{
	ft_assert(yacc != NULL);
	if (yacc->subshell_depth > 0)
		yacc->subshell_depth--;
}

int	yacc_in_subshell(t_yacc *yacc)
{
	ft_assert(yacc != NULL);
	return (yacc->subshell_depth > 0);
}

/**
 * Remap token types from system/word to arithmetic equivalents
 * when inside an arithmetic context
 */
t_token_type	yacc_remap_arith_token(t_token_type type)
{
	switch (type)
	{
		case TOKEN_IDENTIFIER:
		case TOKEN_WORD:
			return (TOKEN_IDENTIFIER);
		case TOKEN_PIPE:
			return (TOKEN_OP_BITWISE_OR);
		case TOKEN_AMPERSAND:
			return (TOKEN_OP_BITWISE_AND);
		case TOKEN_OPERATOR:
			return (TOKEN_OPERATOR);
		case TOKEN_SEMICOLON:
			return (TOKEN_SEMICOLON);
		default:
			return (type);
	}
}
