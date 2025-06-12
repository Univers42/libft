/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 10:18:46 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/11 11:20:40 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sort.h"

static int	normalized_value(int val, int min, int max, char *stat)
{
	if (strcmp(stat, "denormalize") == 0)
		return (val * (max - min) / 100 + min);
	else if (strcmp(stat, "normalize") == 0)
		return ((val - min) * 100 / (max - min));
	return (val);
}

static int	compare_ints(const void *a, const void *b)
{
	int	ia;
	int	ib;

	ia = *(const int *)a;
	ib = *(const int *)b;
	return ((ia > ib) - (ia < ib));
}

static void	process_chunks(int *normalized_arr, int size, int num_chunks)
{
	int	chunk_size;
	int	**chunks;
	int	*chunk_counts;
	int	i;
	int	j;
	int	k;
	int	output_idx;

	chunk_size = 100 / num_chunks + 1;
	chunks = malloc(num_chunks * sizeof(int *));
	chunk_counts = calloc(num_chunks, sizeof(int));
	i = 0;
	while (i < num_chunks)
	{
		chunks[i] = malloc(size * sizeof(int));
		i++;
	}
	
	// Distribute elements into chunks
	i = 0;
	while (i < size)
	{
		j = normalized_arr[i] / chunk_size;
		if (j >= num_chunks)
			j = num_chunks - 1;
		chunks[j][chunk_counts[j]++] = normalized_arr[i];
		i++;
	}
	
	// Sort each chunk and merge back
	output_idx = 0;
	i = 0;
	while (i < num_chunks)
	{
		if (chunk_counts[i] > 1)
			qsort(chunks[i], chunk_counts[i], sizeof(int), compare_ints);
		k = 0;
		while (k < chunk_counts[i])
		{
			normalized_arr[output_idx++] = chunks[i][k];
			k++;
		}
		free(chunks[i]);
		i++;
	}
	free(chunks);
	free(chunk_counts);
}

void	ft_chunk_sort(int *arr, int size)
{
	int	*normalized_arr;
	int	min;
	int	max;
	int	i;

	if (!arr || size <= 1)
		return ;
	normalized_arr = malloc(size * sizeof(int));
	if (!normalized_arr)
		return ;
	min = get_min(arr, size);
	max = get_max(arr, size);
	i = 0;
	while (i < size)
	{
		normalized_arr[i] = normalized_value(arr[i], min, max, "normalize");
		i++;
	}
	if (size <= 10)
		process_chunks(normalized_arr, size, 2);
	else if (size <= 50)
		process_chunks(normalized_arr, size, 5);
	else
		process_chunks(normalized_arr, size, 10);
	i = 0;
	while (i < size)
	{
		arr[i] = normalized_value(normalized_arr[i], min, max, "denormalize");
		i++;
	}
	free(normalized_arr);
}

// int main(void)
// {
//     int arr[] = {2, 802, 24, 45, 66, 170, 75, 90};
//     int size = 8;
//
//     printf("Original array: ");
//     print_array(arr, size);
//
//     ft_chunk_sort(arr, size);
//
//     printf("Sorted array: ");
//     print_array(arr, size);
//
//     return (0);
// }