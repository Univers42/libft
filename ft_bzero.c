/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 14:30:26 by dlesieur          #+#    #+#             */
/*   Updated: 2025/01/26 19:43:04 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/**
 * The function erases the data in the specified number of bytes (`n`)
 * of memory starting at the location pointed to by `s`.
 * It achieves this by writing zeros (`0`) to that memory area.
 * 
 * @param s Pointer to the memory area to be zeroed.
 * @param n Number of bytes to set to zero.
 */
void	ft_bzero(void *s, t_size n)
{
	ft_memset(s, 0, n);
}

// Function to generate a random array of integers
//int main(void) {
//    printf("Testing ft_bzero ---\n");
//    for (int j = 0; j < 10; ++j)
//	{
//		test_ft_bzero();	
//	}
//	return 0;
//}