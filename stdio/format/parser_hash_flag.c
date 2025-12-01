/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_hash_flag.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 00:45:41 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/01 01:33:27 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "format.h"

void alt_hex(t_format_parser *p, size_t len, bool upper, unsigned int value)
{
	if (p->token_meta.flags & FLAG_HASH && len > 0 && value != 0)
	{
		writer_putchar(p->writer, '0');
		if (upper)
			writer_putchar(p->writer, 'X');
		else
			writer_putchar(p->writer, 'x');
	}
}
