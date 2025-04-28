/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils_format_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:00:00 by dyl-syzygy        #+#    #+#             */
/*   Updated: 2025/03/24 21:58:31 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_printf_bonus.h"

/**
 * Prints prefix components - character and/or string
 */
t_ssize	ft_print_prefix(char prefix_char, char *prefix_str, int prefix_str_len)
{
	t_ssize	count;

	count = 0;
	if (prefix_char)
		count += ft_print_c(prefix_char);
	if (prefix_str)
		count += write(1, prefix_str, (t_size)prefix_str_len);
	return (count);
}
