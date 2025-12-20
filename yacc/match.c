/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   match.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 18:07:52 by marvin            #+#    #+#             */
/*   Updated: 2025/12/19 18:07:52 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

bool	match(t_scanner *scan, char expected)
{
	if (scan_is_at_end(scan))
		return (false);
	if (*(scan->current) != expected)
		return (false);
	scan->current++;
	return (true);
}
