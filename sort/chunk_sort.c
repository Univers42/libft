/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 21:57:14 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/11 02:48:04 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 21:57:14 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/09 22:30:07 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sort.h"

static int normalized_value(int val, int min, int max, char *stat)
{
    if (strcmp(stat, "denormalize") == 0)
        return (val * (max - min) / 100 + min);  // Fixed formula
    else if (strcmp(stat, "normalize") == 0)
        return ((val - min) * 100 / (max - min));
    return (val);
}

// Simple comparison function for qsort
static int compare_ints(const void *a, const void *b)
{
    int ia = *(const int*)a;
    int ib = *(const int*)b;
    return (ia > ib) - (ia < ib);
}

// Chunk sort implementation using normalized values
static void chunk_sort_normalized(int *normalized_arr, int size, int num_chunks)
{
    int **chunks;
    int *chunk_counts;
    int chunk_size;
    int i, j, chunk_idx, output_idx;

    // Calculate chunk size
    chunk_size = 100 / num_chunks;
    
    // Allocate memory for chunks
    chunks = malloc(num_chunks * sizeof(int *));
    chunk_counts = calloc(num_chunks, sizeof(int));
    
    i = 0;
    while (i < num_chunks)
    {
        chunks[i] = malloc(size * sizeof(int));
        i++;
    }

    // Distribute elements into chunks based on normalized values
    i = 0;
    while (i < size)
    {
        chunk_idx = normalized_arr[i] / chunk_size;
        if (chunk_idx >= num_chunks)
            chunk_idx = num_chunks - 1;
        
        chunks[chunk_idx][chunk_counts[chunk_idx]] = normalized_arr[i];
        chunk_counts[chunk_idx]++;
        i++;
    }

    // Sort each chunk using qsort
    i = 0;
    while (i < num_chunks)
    {
        if (chunk_counts[i] > 1)
            qsort(chunks[i], chunk_counts[i], sizeof(int), compare_ints);
        i++;
    }

    // Merge chunks back into original array
    output_idx = 0;
    i = 0;
    while (i < num_chunks)
    {
        j = 0;
        while (j < chunk_counts[i])
        {
            normalized_arr[output_idx++] = chunks[i][j];
            j++;
        }
        i++;
    }

    // Free allocated memory
    i = 0;
    while (i < num_chunks)
    {
        free(chunks[i]);
        i++;
    }
    free(chunks);
    free(chunk_counts);
}

static int *normalized_sort(int *arr, int size, int num_chunks)
{
    int i;
    int *normalized_arr;
    int max;
    int min;

    normalized_arr = malloc(size * sizeof(int));
    if (!normalized_arr)
        return (NULL);
    
    max = get_max(arr, size);
    min = get_min(arr, size);
    
    // Normalize values to 0-100 range
    i = 0;
    while (i < size)
    {
        normalized_arr[i] = normalized_value(arr[i], min, max, "normalize");
        i++;
    }
    
    // Apply chunk sorting on normalized values
    chunk_sort_normalized(normalized_arr, size, num_chunks);
    
    // Denormalize back to original range
    i = 0;
    while (i < size)
    {
        normalized_arr[i] = normalized_value(normalized_arr[i], min, max, "denormalize");
        i++;
    }
    
    return (normalized_arr);
}

// Public function to perform chunk sort
void ft_chunk_sort(int *arr, int size)
{
    int *sorted_arr;
    int num_chunks;
    int i;

    if (!arr || size <= 1)
        return;

    // Determine number of chunks based on array size
    if (size <= 10)
        num_chunks = 2;
    else if (size <= 50)
        num_chunks = 5;
    else
        num_chunks = 10;

    sorted_arr = normalized_sort(arr, size, num_chunks);
    if (!sorted_arr)
        return;

    // Copy sorted result back to original array
    i = 0;
    while (i < size)
    {
        arr[i] = sorted_arr[i];
        i++;
    }

    free(sorted_arr);
}

//int main(void)
//{
//    int arr[] = {2, 802, 24, 45, 66, 170, 75, 90};
//    int size = 8;
//
//    printf("Original array: ");
//    print_array(arr, size);
//
//    ft_chunk_sort(arr, size);
//
//    printf("Sorted array: ");
//    print_array(arr, size);
//
//    return (0);
//}