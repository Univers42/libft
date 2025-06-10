/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_sort.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 17:25:42 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/10 17:28:32 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "ft_sort.h"

void    heap_sort()
{

}

// Helper function to print array
static void	print_array(int *arr, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		printf("%d ", arr[i]);
		i++;
	}
	printf("\n");
}

int	main(void)
{
	int	arr[] = {34, 2, 10, -9, 50, 3, 40, 7};
	int	size = 8;

	printf("Original array: ");
	print_array(arr, size);
	ft_gnome_sort(arr, size);
	printf("Sorted array: ");
	print_array(arr, size);
	return (0);
}