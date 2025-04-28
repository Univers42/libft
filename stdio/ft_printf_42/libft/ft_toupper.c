/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 01:02:10 by dyl-syzygy        #+#    #+#             */
/*   Updated: 2025/03/23 07:57:22 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "./TDD/test.h"
#include "libft.h"
/**
 * Converts a lowercase letter to its uppercase equivalent.
 * 
 * @param c The character to convert.
 * @return 
 * The uppercase equivalent of `c` if `c` is a 
 * lowercase letter; otherwise, returns `c` unchanged.
 * 
 * @note If `c` is not a lowercase letter 
 * (i.e., not between 'a' and 'z'), it is returned as is.
 * This function is typically used to standardize 
 * characters in a case-insensitive context.
 */
int	ft_toupper(int c)
{
	if (ft_islower(c))
		return (c - ('a' - 'A'));
	return (c);
}
