/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vnewprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 22:53:29 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/04 23:19:31 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "format.h"
#include <stdlib.h>

int ft_veprintf(const char *str, va_list *args)
{
	t_writer writer;
	t_format_parser parser;
	int ret;

	if (!str)
		return (-1);
	writer_init_fd(&writer, 2);
	ret = parser_parse_and_write(&parser, str, args, &writer);
	return (ret);
}

char *ft_vaseprintf(const char *str, va_list *args)
{
	t_writer writer;
	t_format_parser parser;
	char *result;
	int len;

	if (!str)
		return (NULL);
	writer_init_buf(&writer, NULL, 0);
	len = parser_parse_and_write(&parser, str, args, &writer);
	if (len < 0)
		return (NULL);
	result = malloc((size_t)len + 1);
	if (!result)
		return (NULL);
	writer_init_buf(&writer, result, (size_t)len);
	parser_parse_and_write(&parser, str, args, &writer);
	result[len] = '\0';
	return (result);
}