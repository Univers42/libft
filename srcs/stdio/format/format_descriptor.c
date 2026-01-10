/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_descriptor.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 15:10:50 by syzygy            #+#    #+#             */
/*   Updated: 2025/12/01 01:33:27 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "format.h"

int ft_vdprintf(int fd, const char *format, va_list *ap)
{
	static t_writer buf_out;
	int return_value;
	t_format_parser parser;

	if (format == NULL || fd < 0)
		return (-1);
	writer_init_fd(&buf_out, fd);
	return_value = parser_parse_and_write(&parser, format, ap, &buf_out);
	return (return_value);
}

/**
 * In pending
 */
int ft_vlogprintf(t_state_fd *state, const char *format, va_list *ap)
{
	static t_writer buf_out;
	int return_value;
	t_format_parser parser;

	if (format == NULL || !state || state->fd < 0)
		return (-1);
	writer_init_fd(&buf_out, state->fd);
	return_value = parser_parse_and_write(&parser, format, ap, &buf_out);
	return (return_value);
}
