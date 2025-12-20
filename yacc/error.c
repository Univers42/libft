/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 18:09:42 by marvin            #+#    #+#             */
/*   Updated: 2025/12/19 18:09:42 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_token	error_token(t_scanner *scan, const char *mesg)
{
	t_token	token;

	token.type = TOKEN_ERR;
	token.start = mesg;
	token.len = (int)strlen(mesg);
	token.line = scan->line;
	return (token);
}
