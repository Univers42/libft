/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bubble_sort.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 21:57:08 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/09 22:44:24 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sort.h"
#include <stdbool.h>
#include <stdio.h>

void    ft_swap(int *a, int *b)
{
    if (!a || !b || a == b)
        return ;
    (*a ^= *b), (*b ^= *a), (*a ^= *b);
}

int *ft_bubble_sort(int *arr, int size)
{
    int i;
    int j;
    bool swapped;

    if (!arr || size <= 1)
        return (arr);

    i = 0;
    while (i < size - 1)
    {
        swapped = false;
        j = 0;
        while (j < size - i - 1)
        {
            if (arr[j] > arr[j + 1])
            {
                ft_swap(&arr[j], &arr[j + 1]);
                swapped = true;
            }
            j++;
        }
        i++;
        if (!swapped)
            break;
    }
    return (arr);
}

// Helper function to print array
static void print_array(int *arr, int size)
{
    int i;
    
    i = 0;
    while (i < size)
    {
        printf("%d ", arr[i]);
        i++;
    }
    printf("\n");
}

int main(void)
{
    int arr[] = {64, 34, 25, 12, 22, 11, 90};
    int size = 7;

    printf("Original array: ");
    print_array(arr, size);

    ft_bubble_sort(arr, size);

    printf("Sorted array: ");
    print_array(arr, size);

    return (0);
}