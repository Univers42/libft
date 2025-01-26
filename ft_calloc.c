/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 14:12:12 by dlesieur          #+#    #+#             */
/*   Updated: 2025/01/26 19:44:10 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <stddef.h>
#include <limits.h>
// Calcular dinámicamente SIZE_MAX sin macros ni bucles for
static t_size	size_max(void)
{
	t_size	size_max;
	t_size	bit_count;
	t_size	i;

	size_max = 0;
	bit_count = sizeof(t_size) * 8;
	i = 0;
	while (i < bit_count)
	{
		size_max |= (t_size)1 << i;
		i++;
	}
	return (size_max);
}

/**
 * Allocates memory for an array of `nmemb` elements, each of `size` bytes,
 * and initializes all bytes in the allocated memory to zero.
 * 
 * @param nmemb Number of elements to allocate.
 * @param size Size of each element in bytes.
 * @return A pointer to the allocated memory, or NULL if the allocation fails.
 */
void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	total;
	void	*buf;

	if (nmemb && size > size_max() / nmemb)
		return (NULL);
	total = nmemb * size;
	buf = malloc(total);
	if (!buf)
		return (NULL);
	ft_bzero(buf, total);
	return (buf);
}
//int main(void)
//{
//	int n = 5;
//	int *arr;
//	arr = (int *)calloc(n, sizeof(int));
//	if (!arr)
//	{
//		printf("Memory allocation failed.\n");
//		return 1;
//	}
//	printf("Array elements after calloc: \n");
//	for(int i = 0; i < n; i++)
//		printf("%d ", arr[i]);
//	printf("\n");
//	for(int i = 0; i < n; i++)
//		arr[i] = i + 1;
//	printf("Array elements after settings values: \n");
//	for (int i = 0; i < n; i++)
//		printf("%d ", arr[i]);
//	printf("\n");
//	free(arr);
//	return (0);
//}