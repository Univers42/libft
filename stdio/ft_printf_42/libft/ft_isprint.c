/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 16:08:48 by dlesieur          #+#    #+#             */
/*   Updated: 2025/01/26 19:45:05 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
//#include "TDD/test.h"
/**
 * Checks if the given character is printable, including space.
 * Printable characters are those with values between 32 
 * and 126 (inclusive) in the ASCII table.
 * 
 * @param c The character to check (represented as an int, 
 * typically from an unsigned char or EOF).
 * @return 1 if the character is printable, 0 otherwise.
 */
int	ft_isprint(int c)
{
	return (c >= ' ' && c <= '~');
}
