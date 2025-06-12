/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bitonic_sort.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 10:18:25 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/11 11:20:38 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sort.h"

static void	bitonic_merge(int *arr, int low, int cnt, int dir)
{
	int	i;
	int	k;

	if (cnt > 1)
	{
		k = cnt / 2;
		i = low;
		while (i < low + k)
		{
			if ((dir && arr[i] > arr[i + k]) || (!dir && arr[i] < arr[i + k]))
				ft_swap(&arr[i], &arr[i + k], sizeof(int));
			i++;
		}
		bitonic_merge(arr, low, k, dir);
		bitonic_merge(arr, low + k, k, dir);
	}
}

static void	bitonic_sort_rec(int *arr, int low, int cnt, int dir)
{
	int	k;

	if (cnt > 1)
	{
		k = cnt / 2;
		bitonic_sort_rec(arr, low, k, 1);
		bitonic_sort_rec(arr, low + k, k, 0);
		bitonic_merge(arr, low, cnt, dir);
	}
}

void	bitonic_sort(int *arr, int size)
{
	int	power_of_2;
	int	*padded_arr;
	int	i;

	if (!arr || size <= 1)
		return ;
	
	// Find next power of 2
	power_of_2 = 1;
	while (power_of_2 < size)
		power_of_2 *= 2;
	
	// If size is already power of 2, sort directly
	if (power_of_2 == size)
	{
		bitonic_sort_rec(arr, 0, size, 1);
		return ;
	}
	
	// Create padded array for non-power-of-2 sizes
	padded_arr = malloc(power_of_2 * sizeof(int));
	if (!padded_arr)
		return ;
	
	// Copy original data and pad with maximum values
	i = 0;
	while (i < size)
	{
		padded_arr[i] = arr[i];
		i++;
	}
	while (i < power_of_2)
	{
		padded_arr[i] = 2147483647; // INT_MAX
		i++;
	}
	
	bitonic_sort_rec(padded_arr, 0, power_of_2, 1);
	
	// Copy back only the original elements
	i = 0;
	while (i < size)
	{
		arr[i] = padded_arr[i];
		i++;
	}
	
	free(padded_arr);
}
