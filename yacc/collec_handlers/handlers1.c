/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 22:18:54 by marvin            #+#    #+#             */
/*   Updated: 2025/12/19 22:18:54 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_token	handle_newline(t_scanner *scan)
{
	advance(scan);
	return (make_token(scan, TOKEN_NEWLINE));
}

t_token	handle_squote(t_scanner *scan)
{
	return (string(scan));
}

t_token	handle_dquote(t_scanner *scan)
{
	return (string(scan));
}

t_token	handle_bquote(t_scanner *scan)
{
	return (string(scan));
}

t_token	handle_sys_op(t_scanner *scan)
{
	return (make_token(scan, TOKEN_EOF));
}
