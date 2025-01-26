/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 12:32:35 by dlesieur          #+#    #+#             */
/*   Updated: 2025/01/26 19:44:36 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
//#include "TDD/test.h"
/**
 * Checks if the given character is alphanumeric.
 * An alphanumeric character is either a digit (0-9) or a letter (A-Z, a-z).
 * 
 * @param c The character to check (represented as an int, typically 
 * çfrom an unsigned char or EOF).
 * @return 1 if the character is alphanumeric, 0 otherwise.
 */
int	ft_isalnum(int c)
{
	return (ft_isalpha(c) || ft_isdigit(c));
}
