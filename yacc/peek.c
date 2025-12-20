/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   peek.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 17:51:17 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/19 18:16:14 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

char	peek(t_scanner *scan)
{
	return (*(scan->current));
}

char	peek_next(t_scanner *scan)
{
	if (scan_is_at_end(scan))
		return ('\0');
	return (scan->current[1]);
}
