/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coktail_shaker_sort.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 16:15:27 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/10 16:45:33 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sort.h"
#include "stdio.h"
/**
 * bi directional bubble sort
 * it is a variation of bubble sort that's easier to understand and implement
 * 
 */


 static void swap (int *a, int *b)
{
    if (!a || !b || a == b)
        return ;
        (*a ^= *b), (*b ^= *a), (*a ^= *b);
}

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
void    print_array(int *arr, int size)
{
    int i;

    i = -1;
    while (++i < size)
        printf("[%d] ==> %d\n",i,  arr[i]);
}

int main(void)
{
    int arr[] = {32, 1, 12, 53, 41};
    int size = sizeof(arr) / sizeof(arr[0]);
    print_array(arr, size);
    cocktail_shaker_sort(arr, size);
    print_array(arr, size);
}