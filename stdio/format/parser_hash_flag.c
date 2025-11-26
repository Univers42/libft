/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_hash_flag.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 00:45:41 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/21 15:33:43 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "format.h"

void	alt_hex(t_parser *p, size_t len, bool upper, unsigned int value)
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
