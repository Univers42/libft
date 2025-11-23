/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_length.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 01:54:35 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/21 15:34:27 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "format.h"

size_t	hex_len(size_t n)
{
	size_t	len;

	len = 1;
	while (n >= 16)
	{
		n /= 16;
		len++;
	}
	return (len);
}

size_t	uint_len(unsigned int n)
{
	size_t	len;

	len = 1;
	while (n >= 10)
	{
		n /= 10;
		len++;
	}
	return (len);
}
