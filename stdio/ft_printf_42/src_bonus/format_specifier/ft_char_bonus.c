/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_char_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 00:50:58 by syzygy            #+#    #+#             */
/*   Updated: 2025/03/24 21:36:33 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_printf_bonus.h"

/**
 * ##### Handles printing a character with formatting
 * 
 * @param c The character to print
 * @param flags The formatting flags
 * @return Number of characters printed
 * @note This function is called by the jump table
 * @note It is used to print a character with a formatting
 * @note It handles width and left alignment
 * @note so first flags_bits is checked for LEFT_FLAG
 * @note if it is set, the character is printed first.
 * @note otherwise, the width is printed first
 */
int	ft_print_char(int c, t_flags flags)
{
	int	count;

	count = 0;
	if (flags.flags_bits & LEFT_FLAG)
	{
		count += ft_print_c((char)c);
		count += ft_print_width(flags.width - 1, 0, 0);
	}
	else
	{
		count += ft_print_width(flags.width - 1, 0, 0);
		count += ft_print_c((char)c);
	}
	return (count);
}
