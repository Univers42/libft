/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 12:43:15 by dlesieur          #+#    #+#             */
/*   Updated: 2025/01/26 19:47:58 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
//#include "TDD/test.h"
/**
 * Compares the first `n` bytes of two memory areas `s1` and `s2`.
 * 
 * @param s1 The first memory area to compare.
 * @param s2 The second memory area to compare.
 * @param n The number of bytes to compare.
 * @return 
 * An integer less than, equal to, or greater 
 * than zero if the first `n` bytes of `s1` 
 * are found to be less than, equal to, or greater than 
 * those of `s2`, respectively.
 * 
 * @note The comparison is done byte-by-byte, 
 * and the result is based on the difference between 
 * the first differing byte. If no differences are found, zero is returned.
 */
int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		if (((unsigned char *)s1)[i] != ((unsigned char *)s2)[i])
			return (((unsigned char *)s1)[i] - ((unsigned char *)s2)[i]);
		i++;
	}
	return (0);
}
//int main(void)
//{
//	char str[] = "hello world!";
//	char str2[] = "hello, world!";
//	int arr[] = {156,4,89,854,312,15,54};
//	int arr2[] = {156,4,89,86,146,4,87};
//	int cmp1 = ft_memcmp((char *)str, (char *)str2, 8);
//	int cmp2 = ft_memcmp((int *)arr, (int *)arr2, 6);	
//	if(cmp1)
//		printf("difference found where the pointer 
//point out, meaning here  %d", cmp1);
//	else
//		printf("The literals are equal !");
//	return 0;
//}