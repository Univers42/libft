/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insertion_sort.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 21:57:40 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/10 13:10:22 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "ft_sort.h"

int *insertion_sort(int *arr, int size)
{
    int i;
    int j;
    int key;

    i = 1;
    while (i < size)
    {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
        i++;
    }
    return arr;
}

void    print_array(int *arr, int size)
{
    int i;

    i = 0;
    while(i < size)
    {
        printf("[%d] ==> %d\n", size, arr[i]);
        i++;
    }
}

int main(void)
{
    int arr[7] = {25 ,0, 5, 1, 50, 42, 40};
    int size = 7;
    print_array(arr, size);
    insertion_sort(arr, size);
    print_array(arr, size);
    return (0);   
}