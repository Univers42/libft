/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cycle_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 17:07:55 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/11 11:23:22 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sort.h"

static int	find_correct_position(int *arr, int size, int value)
{
	int	pos;
	int	i;

	pos = 0;
	i = 0;
	while (i < size)
	{
		if (arr[i] < value)
			pos++;
		i++;
	}
	return (pos);
}

static void	handle_duplicate_values(int *arr, int *pos, int value, int size)
{
	while (*pos < size && arr[*pos] == value)
		(*pos)++;
}

int	ft_cycle_sort(int *arr, int size)
{
	int	writes;
	int	cycle_start;
	int	item;
	int	pos;
	int	temp;

	if (!arr || size <= 1)
		return (0);
	writes = 0;
	cycle_start = 0;
	while (cycle_start < size - 1)
	{
		item = arr[cycle_start];
		pos = cycle_start;
		
		// Count how many elements are smaller than item
		pos = find_correct_position(arr + cycle_start + 1, 
			size - cycle_start - 1, item) + cycle_start;
		
		// If item is already in correct position
		if (pos == cycle_start)
		{
			cycle_start++;
			continue ;
		}
		
		// Skip duplicates
		handle_duplicate_values(arr, &pos, item, size);
		
		// Place item in correct position
		if (pos < size && item != arr[pos])
		{
			temp = item;
			item = arr[pos];
			arr[pos] = temp;
			writes++;
		}
		
		// Continue the cycle
		while (pos != cycle_start && pos < size)
		{
			pos = find_correct_position(arr, size, item);
			handle_duplicate_values(arr, &pos, item, size);
			
			if (pos < size && item != arr[pos])
			{
				temp = item;
				item = arr[pos];
				arr[pos] = temp;
				writes++;
			}
		}
		cycle_start++;
	}
	return (writes);
}

//int	main(void)
//{
//	int	arr2[] = {20, 30, 10, 40, 60, 50, 5};
//	int	size2;
//	int	writes;
//
//	size2 = 7;
//	printf("Original array: ");
//	print_array(arr2, size2);
//	writes = ft_cycle_sort(arr2, size2);
//	printf("Sorted array: ");
//	print_array(arr2, size2);
//	printf("Total writes: %d\n", writes);
//	return (0);
//}
