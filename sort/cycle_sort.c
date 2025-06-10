/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cycle_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 17:07:55 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/10 17:22:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sort.h"
#include <stdio.h>
#include <stdlib.h>

static void	print_array(int *arr, int size);
static int	find_position(int *arr, int size, int item, int start);
static void	handle_duplicates(int *arr, int *pos, int item);
static int	place_item(int *arr, int *item, int pos, int start);
static int	complete_cycle(int *arr, int size, int *item, int start);

static int	find_position(int *arr, int size, int item, int start)
{
	int	pos;
	int	i;

	pos = start;
	i = start + 1;
	while (i < size)
	{
		if (arr[i] < item)
			pos++;
		i++;
	}
	return (pos);
}

static void	handle_duplicates(int *arr, int *pos, int item)
{
	while (item == arr[*pos])
		(*pos)++;
}

static int	place_item(int *arr, int *item, int pos, int start)
{
	int	temp;

	if (pos != start)
	{
		temp = *item;
		*item = arr[pos];
		arr[pos] = temp;
		return (1);
	}
	return (0);
}

static int	complete_cycle(int *arr, int size, int *item, int start)
{
	int	pos;
	int	writes;

	writes = 0;
	pos = find_position(arr, size, *item, start);
	handle_duplicates(arr, &pos, *item);
	writes += place_item(arr, item, pos, start);
	while (pos != start)
	{
		pos = find_position(arr, size, *item, start);
		handle_duplicates(arr, &pos, *item);
		if (*item != arr[pos])
			writes += place_item(arr, item, pos, start);
	}
	return (writes);
}

int	ft_cycle_sort(int *arr, int size)
{
	int	writes;
	int	cycle_start;
	int	item;
	int	pos;

	if (!arr || size <= 1)
		return (0);
	writes = 0;
	cycle_start = 0;
	while (cycle_start <= size - 2)
	{
		item = arr[cycle_start];
		pos = find_position(arr, size, item, cycle_start);
		if (pos == cycle_start)
		{
			cycle_start++;
			continue ;
		}
		handle_duplicates(arr, &pos, item);
		writes += place_item(arr, &item, pos, cycle_start);
		writes += complete_cycle(arr, size, &item, cycle_start);
		cycle_start++;
	}
	return (writes);
}

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
	int	arr2[] = {20, 30, 10, 40, 60, 50, 5};
	int	size2;
	int	writes;

	size2 = 7;
	printf("Original array: ");
	print_array(arr2, size2);
	writes = ft_cycle_sort(arr2, size2);
	printf("Sorted array: ");
	print_array(arr2, size2);
	printf("Total writes: %d\n", writes);
	return (0);
}
