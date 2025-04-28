/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 16:05:11 by dlesieur          #+#    #+#             */
/*   Updated: 2025/01/27 21:29:46 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
//#include "TDD/test.h"
/**
 * Checks if the given character is part of the ASCII character set.
 * The ASCII character set includes characters with values from 0 to 127.
 * 
 * @param c The character to check (represented as an int, 
 * typically from an unsigned char or EOF).
 * @return 1 if the character is in the ASCII range, 0 otherwise.
 */
int	ft_isascii(int c)
{
	return (c >= 0 && c < 128);
}
