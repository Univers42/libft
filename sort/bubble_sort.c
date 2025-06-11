/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bubble_sort.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 21:57:08 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/11 02:24:35 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sort.h"

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

//int main(void)
//{
//    int arr[] = {64, 34, 25, 12, 22, 11, 90};
//    int size = 7;
//
//    printf("Original array: ");
//    print_array(arr, size);
//
//    ft_bubble_sort(arr, size);
//
//    printf("Sorted array: ");
//    print_array(arr, size);
//
//    return (0);
//}