/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coktail_shaker_sort.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 16:15:27 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/11 02:47:26 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sort.h"
/**
 * bi directional bubble sort
 * it is a variation of bubble sort that's easier to understand and implement
 * 
 */

void	cocktail_shaker_sort(int *arr, int size)
{
    int	start;
    int	swapped;
    int	i;
    int	end;

    if (!arr || size <= 1)
        return ;
    start = 0;
    end = size - 1;
    swapped = 1;
    while (swapped)
    {
        swapped = 0;
        i = start;
        while (i < end)
        {
            if (arr[i] > arr[i + 1])
            {
                swap(&arr[i], &arr[i + 1]);
                swapped = 1;
            }
            i++;
        }
        if (!swapped)
            break ;
        end--;
        i = end;
        while (i > start)
        {
            if (arr[i] < arr[i - 1])
            {
                swap(&arr[i], &arr[i - 1]);
                swapped = 1;
            }
            i--;
        }
        start++;
    }
}

//int main(void)
//{
//    int arr[] = {32, 1, 12, 53, 41};
//    int size = sizeof(arr) / sizeof(arr[0]);
//    print_array(arr, size);
//    cocktail_shaker_sort(arr, size);
//    print_array(arr, size);
//}